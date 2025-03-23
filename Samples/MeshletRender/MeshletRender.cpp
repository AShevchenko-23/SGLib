//*********************************************************
//
// Copyright (c) 2024 Aleksei Shevchenko.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// This application contains parts of code and data
// taken from Microsoft's MeshletRender sample
// that are under the MIT License (MIT)
//
//*********************************************************

#include "MeshletRender.h"

const char* c_meshShaderFilename = "MeshletMS.cso";
const char* c_pixelShaderFilename = "MeshletPS.cso";
const char* c_meshFilename = "Dragon_LOD0.bin";

const SG_COLOR_4F ClearColor = { 0.0f, 0.2f, 0.4f, 1.0f };

MeshletRender::MeshletRender(U32 width, U32 height, wchar_t const* windowName)
    : ISGSample(width, height, windowName)
    , m_Viewport{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f }
    , m_Scissor{ 0, 0, static_cast<int>(width), static_cast<int>(height) }

    , m_pDevice(SG_NULL)
    , m_pExecutionContext(SG_NULL)
    , m_pSwapChain(SG_NULL)

    , m_pPipelineState(SG_NULL)

    , m_pDepthStencil(SG_NULL)
    , m_pDSView(SG_NULL)

    , m_Camera(false)
    , m_FrameIndex(0)
    , m_CurrentAngle(0.0f)
{
    m_Camera.SetFOV(DEG2RAD(60.0f));
    m_Camera.SetZRange(1.0f, 1000.0f);
    m_Camera.SetAspectRatio((float)m_Width / m_Height);
}

void MeshletRender::OnInit()
{
    ISGAdapter* pAdapter = nullptr;
    if (GetVideoAdapter(&pAdapter) != SG_OK)
    {
        throw std::exception("Compatible adapter was not found");
    }

    SG_ADAPTER_INFO_EXECUTION info;
    pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_EXECUTION, &info);

    if (info.MeshShaderTier == SG_MESH_SHADER_TIER_NOT_SUPPORTED)
        throw std::exception("Adapter doesn't support mesh shading");

    SG_EXECUTION_CONTEXT_DESC execCtxDesc{};
    execCtxDesc.FrameBuffers = NumFrames;
    execCtxDesc.QueueCount = 1;
    execCtxDesc.QueueTypes[0] = SG_QUEUE_TYPE_GRAPHICS;

    SG_DEVICE_DESC deviceDesc{};
    deviceDesc.pExecutionContextDesc = &execCtxDesc;

    if (SgCreateDevice(pAdapter, &deviceDesc, &m_pDevice) != SG_OK)
        throw std::exception("Failed device object creation");

    SG_RELEASE(pAdapter);

    m_pDevice->GetExecutionContext(&m_pExecutionContext);

    SG_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.Format = SG_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.OutputWindow = m_hWnd;
    swapChainDesc.BufferCount = max(NumFrames, 2);
    swapChainDesc.Flags = SG_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    swapChainDesc.SwapEffect = SG_SWAP_EFFECT_SEQUENTIAL;

    if (SgCreateSwapChain(m_pExecutionContext, 0, &swapChainDesc, &m_pSwapChain) != SG_OK)
        throw std::exception("Failed swap chain creation");

    LoadPipelineState();
    LoadAssets();
}

void MeshletRender::OnDestroy()
{
    m_pExecutionContext->WaitForIdle();

    m_Model = {};
    for (int i = 0; i < _countof(m_pConstantBuffers); i++)
        SG_RELEASE(m_pConstantBuffers[i]);

    SG_RELEASE(m_pDSView);
    SG_RELEASE(m_pDepthStencil);

    SG_RELEASE(m_pPipelineState);

    SG_RELEASE(m_pSwapChain);
    SG_RELEASE(m_pExecutionContext);
    SG_RELEASE(m_pDevice);
}

void MeshletRender::LoadPipelineState()
{
    // Create the pipeline state, which includes compiling and loading shaders.
    ByteBuffer msBuffer, psBuffer;

    LoadBinaryFile(c_meshShaderFilename, msBuffer);
    LoadBinaryFile(c_pixelShaderFilename, psBuffer);

    SG_MESH_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.MS = { msBuffer.data(), msBuffer.size() };
    psoDesc.PS = { psBuffer.data(), psBuffer.size() };

    SG_BINDING_TABLE_DESC table{};
    table.ConstantBuffers   = { 0, 0, 2 };
    table.SRVs              = { 0, 0, 4 };
    table.ShaderVisibility  = SG_SHADER_VISIBILITY_ALL;

    psoDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
    psoDesc.RootSignature.Tabular.NumTables = 1;
    psoDesc.RootSignature.Tabular.pTables = &table;

    m_pDevice->CreateMeshPipelineState(&psoDesc, &m_pPipelineState);
}

void MeshletRender::LoadAssets()
{
    // Create depth texture and its DSV.
    {
        SG_TEXTURE_DESC depthTextureDesc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_DEPTH_STENCIL, m_Width, m_Height, SG_FORMAT_D16_UNORM, 1, false, false);
        depthTextureDesc.DefaultValue.Format = SG_FORMAT_D16_UNORM;
        depthTextureDesc.DefaultValue.Depth = 1.0f;

        if (m_pDevice->CreateTexture(&depthTextureDesc, &m_pDepthStencil) != SG_OK)
            throw std::exception("Failed to create depth texture");

        SG_DEPTH_STENCIL_VIEW_DESC dsvDesc = FastViewDesc::AsDepthStencil(SG_FORMAT_D16_UNORM, 0, 0);
        if (m_pDevice->CreateDepthStencilView(m_pDepthStencil, &dsvDesc, &m_pDSView) != SG_OK)
            throw std::exception("Failed to create DSV");
    }

    // Create constant buffers.
    for (U32 i = 0; i < _countof(m_pConstantBuffers); i++)
    {
        SG_BUFFER_DESC cbDesc = FastBufferDesc::Constant(sizeof(Constants));

        if (m_pDevice->CreateBuffer(&cbDesc, &m_pConstantBuffers[i]) != SG_OK)
            throw std::exception("Failed to create constant buffer");
    }

    if (!m_Model.LoadFromFile(c_meshFilename))
        throw std::exception("Failed to load a model");

    // Upload frame
    {
        m_pExecutionContext->BeginFrame();
        ISGCommandList* pCommandList;
        if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
        {
            m_Model.UploadGpuResources(m_pDevice, pCommandList);
            m_pExecutionContext->FinishCommandList(pCommandList);
        }
        m_pExecutionContext->EndFrame();
    }
}

void MeshletRender::OnUpdate()
{
    // Calculate current rotating angle
    const float rotationSpeed = DEG2RAD(0.5f);

    m_TimeScaler.TimeStamp();
    m_CurrentAngle += rotationSpeed * m_TimeScaler.ComputeScale();

    if (m_CurrentAngle > XM_2PI)
        m_CurrentAngle -= XM_2PI;
}

void MeshletRender::OnRender()
{
    m_pExecutionContext->BeginFrame();

    // Update buffer after frame has begun to prevent data race
    {
        m_Camera.SetPosition({ cos(m_CurrentAngle) * 150.0f, 75, sin(m_CurrentAngle) * 150.0f });
        m_Camera.SetTarget({ 0, 75, 0 });
        m_Camera.Update();

        XMMATRIX world = XMMatrixIdentity();

        Constants cbData;
        cbData.World = world;
        cbData.WorldView = XMMatrixTranspose(world * m_Camera.GetView());
        cbData.WorldViewProj = XMMatrixTranspose(world * m_Camera.GetViewProjection());
        cbData.DrawMeshlets = true;

        UploadBuffer(m_pConstantBuffers[m_FrameIndex], &cbData, sizeof(cbData));
    }

    ISGCommandList* pCommandList = nullptr;
    if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
    {
        PopulateCommandList(pCommandList);
        m_pExecutionContext->FinishCommandList(pCommandList);
    }

    m_pExecutionContext->EndFrame1(1, &m_pSwapChain);
    m_FrameIndex = (m_FrameIndex + 1) % NumFrames;
}

void MeshletRender::PopulateCommandList(ISGCommandList* pCommandList)
{
    pCommandList->SetRenderTarget(0, m_pSwapChain->GetCurrentRTV());
    pCommandList->ClearRenderTarget(m_pSwapChain->GetCurrentRTV(), &ClearColor);

    pCommandList->SetDepthStencil(m_pDSView);
    pCommandList->ClearDepthStencil(m_pDSView, SG_CLEAR_FLAG_DEPTH, 1.0f, 0);

    pCommandList->SetViewports(1, &m_Viewport);
    pCommandList->SetScissorRects(1, &m_Scissor);

    pCommandList->SetPipelineState(m_pPipelineState);

    pCommandList->SetConstantBuffer(0, 0, m_pConstantBuffers[m_FrameIndex]);

    for (auto& mesh : m_Model)
    {
        pCommandList->SetShaderResource(0, 0, mesh.VertexResources[0].View.Get());
        pCommandList->SetShaderResource(0, 1, mesh.MeshletResource.View.Get());
        pCommandList->SetShaderResource(0, 2, mesh.UniqueVertexIndexResource.View.Get());
        pCommandList->SetShaderResource(0, 3, mesh.PrimitiveIndexResource.View.Get());

        for (UINT i = 0; i < mesh.MeshletSubsets.size(); i++)
        {
            pCommandList->SetConstantBuffer(0, 1, mesh.MeshletInfoCBs[i].Get());
            pCommandList->DispatchMesh(mesh.MeshletSubsets[i].Count, 1, 1);
        }
    }
}
