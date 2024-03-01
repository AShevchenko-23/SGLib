
//*********************************************************
//
// Copyright (c) 2024 Aleksei Shevchenko.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "AsyncCompute.h"
#include "SGX/Box.h"

#define ASYNC_COMPUTE

const SG_COLOR_4F g_ClearColor = { 0.8f, 0.9f, 1.0f, 1 };

// Custom indices for graphics and compute queues
const U8 g_GfxQueue = 0;
const U8 g_CmpQueue = 1;

AsyncComputeSample::AsyncComputeSample(U32 width, U32 height, wchar_t const* windowName)
    : ISGSample(width, height, windowName)
    , m_Viewport{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f }
    , m_Scissor{ 0, 0, static_cast<int>(width), static_cast<int>(height) }

    , m_pDevice(SG_NULL)
    , m_pExecutionContext(SG_NULL)
    , m_pSwapChain(SG_NULL)
    , m_pGfxPipelineState(SG_NULL)
    , m_pCmpPipelineState(SG_NULL)

    , m_pVertexBuffer(SG_NULL)
    , m_pIndexBuffer(SG_NULL)

    , m_pDepthStencil(SG_NULL)
    , m_pDSView(SG_NULL)

    , m_pSampler(SG_NULL)

    , m_pCbsGfx{}
    , m_pCbsCmp{}

    , m_pTexture(SG_NULL)
    , m_pTextureSRVs{}
    , m_pTextureUAVs{}

    , m_Camera(true)
    , m_FrameIndex(0)
    , m_CurrentAngle(0.0f)

    , m_FrameContext{}
{
    m_Camera.SetFOV(XMConvertToRadians(45.0f));
    m_Camera.SetAspectRatio(m_Width / static_cast<float>(m_Height));
    m_Camera.SetZRange(1.0f, 10.0f);
    m_Camera.SetTarget({ 0, 0, 0 });
}

void AsyncComputeSample::OnInit()
{
    ISGAdapter* pAdapter = SG_NULL;
    if (GetVideoAdapter(&pAdapter) != SG_OK)
        throw std::exception("Compatible adapter was not found");

    // Create device object with an execution context for two queues
    {
        SG_ADAPTER_INFO_EXECUTION executionInfo;
        pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_EXECUTION, &executionInfo);

        if (executionInfo.GraphicsQueues == 0)
            throw std::exception("Adapter doesn't support graphics queue");

        if (executionInfo.ComputeQueues == 0)
            throw std::exception("Adapter doesn't support compute queue");

        SG_EXECUTION_CONTEXT_DESC execCtxDesc{};
        execCtxDesc.FrameBuffers = NumFrames;

#ifdef ASYNC_COMPUTE
        execCtxDesc.QueueCount = 2;
        execCtxDesc.QueueTypes[g_GfxQueue] = SG_QUEUE_TYPE_GRAPHICS;
        execCtxDesc.QueueTypes[g_CmpQueue] = SG_QUEUE_TYPE_COMPUTE;
#else
        execCtxDesc.QueueCount = 1;
        execCtxDesc.QueueTypes[g_GfxQueue] = SG_QUEUE_TYPE_GRAPHICS;
#endif

        SG_DEVICE_DESC deviceDesc{};
        deviceDesc.pExecutionContextDesc = &execCtxDesc;

        if (SgCreateDevice(pAdapter, &deviceDesc, &m_pDevice) != SG_OK)
            throw std::exception("Failed to create device object");

        m_pDevice->GetExecutionContext(&m_pExecutionContext);
    }

    // Create swap chain
    {
        SG_SWAP_CHAIN_DESC swapChainDesc{};
        swapChainDesc.OutputWindow = m_hWnd;
        swapChainDesc.Width = 0;
        swapChainDesc.Height = 0;
        swapChainDesc.Format = SG_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferCount = max(NumFrames, 2);
        swapChainDesc.Flags = SG_SWAP_CHAIN_FLAG_ALLOW_TEARING;
        swapChainDesc.SwapEffect = SG_SWAP_EFFECT_SEQUENTIAL;

        if (SgCreateSwapChain(m_pExecutionContext, 0, &swapChainDesc, &m_pSwapChain) != SG_OK)
            throw std::exception("Failed to create swap chain");
    }

    LoadPipelineStates();
    LoadAssets();
}

void AsyncComputeSample::LoadPipelineStates()
{
    // Create graphics pipeline state
    {
        ByteBuffer vsBuffer;
        ByteBuffer psBuffer;

        if (!LoadBinaryFile("VertexShader.cso", vsBuffer))
            throw std::exception("Failed to load vertex shader");

        if (!LoadBinaryFile("PixelShader.cso", psBuffer))
            throw std::exception("Failed to load pixel shader");

        ISGInputLayout* pInputLayout = SG_NULL;

        // Create input layout
        {
            SG_INPUT_ELEMENT_DESC inputElements[] =
            {
                { "POSITION", 0, SG_FORMAT_R32G32B32_FLOAT, 0, 0, SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, SG_FORMAT_R32G32_FLOAT, 0, 12, SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            };

            if (m_pDevice->CreateInputLayout(_countof(inputElements), inputElements, &pInputLayout) != SG_OK)
                throw std::exception("Failed to create input layout");
        }

        // Configure 2 binding tables for the vertex and pixel stages
        SG_BINDING_TABLE_DESC tables[2] = {};
        {
            // The 1st table is visible for vertex shader only
            tables[0].ShaderVisibility = SG_SHADER_VISIBILITY_VERTEX;
            tables[0].ConstantBuffers = { 0, 0, 1 };    // one constant buffer at the register #0 of space #0

            // The 2nd table is visible for pixel shader only
            tables[1].ShaderVisibility = SG_SHADER_VISIBILITY_PIXEL;
            tables[1].SRVs = { 0, 0, 1 };               // one shader resource at the register #0 of space #0
            tables[1].Samplers = { 0, 0, 1 };           // one sampler at the register #0 of space #0
        }

        SG_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
        pipelineDesc.pInputLayout = pInputLayout;
        pipelineDesc.VS = { vsBuffer.data(), vsBuffer.size() };
        pipelineDesc.PS = { psBuffer.data(), psBuffer.size() };
        pipelineDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
        pipelineDesc.RootSignature.Tabular.NumTables = 2;
        pipelineDesc.RootSignature.Tabular.pTables = tables;

        if (m_pDevice->CreateGraphicsPipelineState(&pipelineDesc, &m_pGfxPipelineState) != SG_OK)
            throw std::exception("Failed to create graphics pipeline state");

        pInputLayout->Release();
    }

    // Create compute pipeline state
    {
        ByteBuffer csBuffer;

        if (!LoadBinaryFile("ComputeShader.cso", csBuffer))
            throw std::exception("Failed to load compute shader");

        // Configure binding tables
        SG_BINDING_TABLE_DESC table = {};
        {
            // Use common access option for non-graphical stages
            table.ShaderVisibility  = SG_SHADER_VISIBILITY_ALL;
            table.ConstantBuffers   = { 0, 0, 1 };  // one constant buffer at the register #0 of space #0
            table.SRVs              = { 0, 0, 1 };  // one shader resource at the register #0 of space #0
            table.UAVs              = { 0, 0, 1 };  // one unordered access resource at the register #0 of space #0
        }

        SG_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};
        pipelineDesc.CS = { csBuffer.data(), csBuffer.size() };
        pipelineDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
        pipelineDesc.RootSignature.Tabular.NumTables = 1;
        pipelineDesc.RootSignature.Tabular.pTables = &table;

        if (m_pDevice->CreateComputePipelineState(&pipelineDesc, &m_pCmpPipelineState) != SG_OK)
            throw std::exception("Failed to create compute pipeline state");
    }
}

void AsyncComputeSample::LoadAssets()
{
    // Create depth stencil
    {
        SG_TEXTURE_DESC depthBufferDesc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_DEPTH_STENCIL, m_Width, m_Height, SG_FORMAT_D16_UNORM, 1, false, false);
        depthBufferDesc.DefaultValue.Format = SG_FORMAT_D16_UNORM;
        depthBufferDesc.DefaultValue.Depth = 1.0f;
        m_pDevice->CreateTexture(&depthBufferDesc, &m_pDepthStencil);
        m_pDepthStencil->SetName("DepthBuffer");

        SG_DEPTH_STENCIL_VIEW_DESC dsvDesc = FastViewDesc::AsDepthStencil(SG_FORMAT_D16_UNORM, 0, 0);
        if (m_pDevice->CreateDepthStencilView(m_pDepthStencil, &dsvDesc, &m_pDSView) != SG_OK)
            throw std::exception("Failed to create depth stencil texture");
    }

    // Create read-write texture
    {
        SG_TEXTURE_DESC desc = FastTextureDesc::Tex2DArray(SG_TEXTURE_TYPE_COMMON, m_Width, m_Height, SG_FORMAT_R32G32B32A32_FLOAT, 1, NumSlices, true, false, true);
        if (m_pDevice->CreateTexture(&desc, &m_pTexture) != SG_OK)
            throw std::exception("Failed to create texture");

        m_pTexture->SetName("RWTexture");

        for (U32 i = 0; i < NumSlices; i++)
        {
            SG_SHADER_RESOURCE_VIEW_DESC viewDesc = FastViewDesc::AsTexture(SG_FORMAT_R32G32B32A32_FLOAT, 0, 1, i, 0);
            if (m_pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pTextureSRVs[i]) != SG_OK)
                throw std::exception("Failed to create texture SRV");
        }

        for (U32 i = 0; i < NumSlices; i++)
        {
            SG_UNORDERED_ACCESS_VIEW_DESC viewDesc = FastViewDesc::AsRWTexture(SG_FORMAT_R32G32B32A32_FLOAT, 0, i, 0);
            if (m_pDevice->CreateUnorderedAccessView(m_pTexture, &viewDesc, &m_pTextureUAVs[i]) != SG_OK)
                throw std::exception("Failed to create texture UAV");
        }
    }

    // Create sampler
    {
        SG_SAMPLER_DESC samplerDesc = {};
        samplerDesc.AddressU = SG_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.AddressV = SG_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.AddressW = SG_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.Filter = SG_ENCODE_BASIC_FILTER(SG_FILTER_TYPE_LINEAR, SG_FILTER_TYPE_LINEAR, SG_FILTER_TYPE_LINEAR, SG_FILTER_REDUCTION_STANDART);
        samplerDesc.MaxLOD = 0;

        if (m_pDevice->CreateSampler(&samplerDesc, &m_pSampler) != SG_OK)
            throw std::exception("Failed to create sampler");
    }

    // Create sets of constant buffers for all frames to prevent data race
    for (U32 i = 0; i < NumFrames; i++)
    {
        SG_BUFFER_DESC cbDesc;

        cbDesc = FastBufferDesc::Constant((U32)sizeof(CommonParameters));
        if (m_pDevice->CreateBuffer(&cbDesc, &m_pCbsGfx[i]) != SG_OK)
            throw std::exception("Failed to create constant buffer");

        cbDesc = FastBufferDesc::Constant((U32)sizeof(JulieSetParameters));
        if (m_pDevice->CreateBuffer(&cbDesc, &m_pCbsCmp[i]) != SG_OK)
            throw std::exception("Failed to create constant buffer");
    }

    // Create vertex and index buffers
    {
        SG_BUFFER_DESC vbDesc = FastBufferDesc::Vertex(sizeof(g_pBoxVertices), false, false, false);
        if (m_pDevice->CreateBuffer(&vbDesc, &m_pVertexBuffer) != SG_OK)
            throw std::exception("Failed to create vertex buffer");

        SG_BUFFER_DESC ibDesc = FastBufferDesc::Index(sizeof(g_pBoxIndices), false, false, false);
        if (m_pDevice->CreateBuffer(&ibDesc, &m_pIndexBuffer) != SG_OK)
            throw std::exception("Failed to create index buffer");
    }

    // Dummy frame to upload vertex and index data
    {
        m_pExecutionContext->BeginFrame();
        ISGCommandList* pCommandList;
        if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
        {
            UploadCommonBuffer(m_pDevice, pCommandList, m_pVertexBuffer, g_pBoxVertices, sizeof(g_pBoxVertices));
            UploadCommonBuffer(m_pDevice, pCommandList, m_pIndexBuffer, g_pBoxIndices, sizeof(g_pBoxIndices));

            m_pExecutionContext->FinishCommandList(pCommandList);
        }

        m_pExecutionContext->EndFrame();
    }
}

void AsyncComputeSample::OnDestroy()
{
    // Wait until all scheduled and submitted GPU work is done
    m_pExecutionContext->WaitForIdle();

    m_FrameContext = {};

    for (int i = 0; i < NumSlices; i++)
    {
        SG_RELEASE(m_pTextureSRVs[i]);
        SG_RELEASE(m_pTextureUAVs[i]);
    }

    SG_RELEASE(m_pTexture);

    for (int i = 0; i < NumFrames; i++)
    {
        SG_RELEASE(m_pCbsGfx[i]);
        SG_RELEASE(m_pCbsCmp[i]);
    }

    SG_RELEASE(m_pSampler);

    SG_RELEASE(m_pDSView);
    SG_RELEASE(m_pDepthStencil);

    SG_RELEASE(m_pIndexBuffer);
    SG_RELEASE(m_pVertexBuffer);

    SG_RELEASE(m_pCmpPipelineState);
    SG_RELEASE(m_pGfxPipelineState);

    SG_RELEASE(m_pSwapChain);
    SG_RELEASE(m_pExecutionContext);
    SG_RELEASE(m_pDevice);
}

void AsyncComputeSample::OnUpdate()
{
    // Set-up current frame context
    m_FrameContext.pGfxCB = m_pCbsGfx[m_FrameIndex % NumFrames];
    m_FrameContext.pCmpCB = m_pCbsCmp[m_FrameIndex % NumFrames];
    m_FrameContext.pSRV = m_pTextureSRVs[m_FrameIndex % NumSlices];
    m_FrameContext.pUAV = m_pTextureUAVs[(m_FrameIndex + 1) % NumSlices];

    // Calculate current rotating angle of the box relating to frame time
    {
        const float rotationSpeed = DEG2RAD(0.5f);

        m_TimeScaler.TimeStamp();
        m_CurrentAngle += rotationSpeed * m_TimeScaler.ComputeScale();

        if (m_CurrentAngle > XM_2PI)
            m_CurrentAngle -= XM_2PI;

        const float camDistance = 5.0f;
        const float camX = cosf(m_CurrentAngle) * camDistance;
        const float camZ = sinf(m_CurrentAngle) * camDistance;
        m_Camera.SetPosition({ camX, 0, camZ });
        m_Camera.Update();
    }
}

void AsyncComputeSample::OnRender()
{
    m_pExecutionContext->BeginFrame();

    // Update buffers after frame has begun to prevent data race
    {
        // Constant buffer for graphics pass
        {
            CommonParameters params;
            params.WorldTransform = XMMatrixRotationZ(m_CurrentAngle * 2);
            params.ViewProjection = m_Camera.GetViewProjection();

            UploadBuffer(m_FrameContext.pGfxCB, &params, sizeof(params));
        }

        // Constant buffer for compute pass
        {
            JulieSetParameters params;
            params.InvTextureSizeX = 1.0f / m_Width;
            params.InvTextureSizeY = 1.0f / m_Height;
            params.Angle = m_CurrentAngle;

            UploadBuffer(m_FrameContext.pCmpCB, &params, sizeof(params));
        }
    }

#ifdef ASYNC_COMPUTE
    ISGCommandList* pGfxCommandList = SG_NULL;
    if (m_pExecutionContext->ScheduleCommandList(g_GfxQueue, 1, &pGfxCommandList) == SG_OK)
    {
        PrepareGraphicsCommandList(pGfxCommandList, &m_FrameContext);
        m_pExecutionContext->FinishCommandList(pGfxCommandList);
    }

    ISGCommandList* pCmpCommandList = SG_NULL;
    if (m_pExecutionContext->ScheduleCommandList(g_CmpQueue, 1, &pCmpCommandList) == SG_OK)
    {
        PrepareComputeCommandList(pCmpCommandList, &m_FrameContext);
        m_pExecutionContext->FinishCommandList(pCmpCommandList);
    }
#else
    ISGCommandList* pCommandList = SG_NULL;
    if (m_pExecutionContext->ScheduleCommandList(g_GfxQueue, 1, &pCommandList) == SG_OK)
    {
        PrepareGraphicsCommandList(pCommandList, &m_FrameContext);
        PrepareComputeCommandList(pCommandList, &m_FrameContext);

        m_pExecutionContext->FinishCommandList(pCommandList);
    }
#endif

    m_pExecutionContext->EndFrame1(1, &m_pSwapChain);
    m_FrameIndex++;
}

void AsyncComputeSample::PrepareGraphicsCommandList(ISGCommandList* pCommandList, FrameContext* pFrameCtx)
{
    pCommandList->SetRenderTarget(0, m_pSwapChain->GetCurrentRTV());
    pCommandList->ClearRenderTarget(m_pSwapChain->GetCurrentRTV(), &g_ClearColor);

    pCommandList->SetDepthStencil(m_pDSView);
    pCommandList->ClearDepthStencilDefault(m_pDSView, SG_CLEAR_FLAG_DEPTH);

    pCommandList->SetViewports(1, &m_Viewport);
    pCommandList->SetScissorRects(1, &m_Scissor);

    pCommandList->SetVertexBuffer(0, m_pVertexBuffer, 0, sizeof(Vertex));
    pCommandList->SetIndexBuffer(m_pIndexBuffer, 0, SG_FORMAT_R16_UINT);

    pCommandList->SetPipelineState(m_pGfxPipelineState);
    pCommandList->SetPrimitiveTopology(SG_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    // Bind constant buffer through the vertex shader's table (see root signature describing above)
    pCommandList->SetConstantBuffer(0, 0, pFrameCtx->pGfxCB);

    // Bind texture and sampler through the pixel shader's table (see root signature describing above)
    pCommandList->SetShaderResource(1, 0, pFrameCtx->pSRV);
    pCommandList->SetSampler(1, 0, m_pSampler);

    pCommandList->DrawIndexedInstanced(_countof(g_pBoxIndices), 512, 0, 0, 0);
}

void AsyncComputeSample::PrepareComputeCommandList(ISGCommandList* pCommandList, FrameContext* pFrameCtx)
{
    pCommandList->SetPipelineState(m_pCmpPipelineState);

    // Bind resources through the only one table (see root signature describing above)
    pCommandList->SetConstantBuffer(0, 0, pFrameCtx->pCmpCB);
    pCommandList->SetUnorderedAccessView(0, 0, pFrameCtx->pUAV);

    pCommandList->Dispatch(m_Width >> 3, m_Height >> 3, 1);
}
