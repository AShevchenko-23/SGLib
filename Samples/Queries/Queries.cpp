//*********************************************************
//
// This code contains parts of Microsoft DirectX Samples
// Which came with the following warning:
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "Queries.h"

struct Vertex
{
    XMFLOAT3 position;
    XMFLOAT4 color;
};

const SG_COLOR_4F g_ClearColor = { 0.0f, 0.2f, 0.4f, 1.0f };

QueriesSample::QueriesSample(U32 width, U32 height, wchar_t const* windowName)
    : ISGSample(width, height, windowName)
    , m_Viewport{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f }
    , m_Scissor{ 0, 0, static_cast<int>(width), static_cast<int>(height) }

    , m_pDevice(SG_NULL)
    , m_pSwapChain(SG_NULL)
    , m_pPipelineState(SG_NULL)
    
    , m_pDepthOnlyRead(SG_NULL)

    , m_pBlendStateRTOverride(SG_NULL)
    , m_pBlendStateNoBlendState(SG_NULL)

    , m_pVertexBuffer(SG_NULL)

    , m_pDepthStencil(SG_NULL)
    , m_pDSView(SG_NULL)

    , m_pCbFarQuad{}
    , m_pCbNearQuad{}

    , m_pPredicate(SG_NULL)

    , m_FrameIndex(0)
{
}

void QueriesSample::OnInit()
{
    ISGAdapter* pAdapter = nullptr;
    if (GetVideoAdapter(&pAdapter) != SG_OK)
        throw std::exception("Compatible adapter was not found");

    SG_EXECUTION_CONTEXT_DESC execCtxDesc{};
    execCtxDesc.FrameBuffers = NumFrames;
    execCtxDesc.QueueCount = 1;
    execCtxDesc.QueueTypes[0] = SG_QUEUE_TYPE_GRAPHICS;

    SG_DEVICE_DESC deviceDesc{};
    deviceDesc.pExecutionContextDesc = &execCtxDesc;

    if (SgCreateDevice(pAdapter, &deviceDesc, &m_pDevice) != SG_OK)
        throw std::exception("Failed device object creation");

    m_pDevice->GetExecutionContext(&m_pExecutionContext);

    SG_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.Format = SG_FORMAT_R8G8B8A8_UNORM;//B8G8R8A8_UNORM;// R16G16B16A16_FLOAT;//
    swapChainDesc.OutputWindow = m_hWnd;
    swapChainDesc.BufferCount = max(NumFrames, 2);
    swapChainDesc.Flags = SG_SWAP_CHAIN_FLAG_ALLOW_TEARING | SG_SWAP_CHAIN_FLAG_ALLOW_SHADER_RESOURCE;
    swapChainDesc.SwapEffect = SG_SWAP_EFFECT_SEQUENTIAL;

    if (SgCreateSwapChain(m_pExecutionContext, 0, &swapChainDesc, &m_pSwapChain) != SG_OK)
        throw std::exception("Failed swap chain creation");

    LoadPipelineState();
    LoadAssets();
}

void QueriesSample::OnDestroy()
{
    m_pExecutionContext->WaitForIdle();

    SG_RELEASE(m_pPredicate);

    for (int i = 0; i < NumFrames; i++)
    {
        SG_RELEASE(m_pCbFarQuad[i]);
        SG_RELEASE(m_pCbNearQuad[i]);
    }

    SG_RELEASE(m_pDSView);
    SG_RELEASE(m_pDepthStencil);

    SG_RELEASE(m_pVertexBuffer);

    SG_RELEASE(m_pBlendStateNoBlendState);
    SG_RELEASE(m_pBlendStateRTOverride);

    SG_RELEASE(m_pDepthOnlyRead);

    SG_RELEASE(m_pPipelineState);

    SG_RELEASE(m_pSwapChain);
    SG_RELEASE(m_pExecutionContext);
    SG_RELEASE(m_pDevice);
}

void QueriesSample::LoadPipelineState()
{
    SG_INPUT_ELEMENT_DESC inputElements[] =
    {
        { "POSITION", 0, SG_FORMAT_R32G32B32_FLOAT, 0, 0, SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, SG_FORMAT_R32G32B32A32_FLOAT, 0, 12, SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    ISGInputLayout* pInputLayout = nullptr;
    if (m_pDevice->CreateInputLayout(_countof(inputElements), inputElements, &pInputLayout) != SG_OK)
        throw std::exception("Failed to create input layout");

    ByteBuffer vsBuffer, psBuffer;

    if (!LoadBinaryFile("VertexShader.cso", vsBuffer))
        throw std::exception("Failed to load vertex shader");

    if (!LoadBinaryFile("PixelShader.cso", psBuffer))
        throw std::exception("Failed to load pixel shader");

    SG_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
    pipelineDesc.pInputLayout = pInputLayout;
    pipelineDesc.VS = { vsBuffer.data(), vsBuffer.size() };
    pipelineDesc.PS = { psBuffer.data(), psBuffer.size() };

    SG_BINDING_TABLE_DESC table{};
    table.ConstantBuffers = { 0, 0, 1 };
    table.ShaderVisibility = SG_SHADER_VISIBILITY_VERTEX;

    pipelineDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
    pipelineDesc.RootSignature.Tabular.NumTables = 1;
    pipelineDesc.RootSignature.Tabular.pTables = &table;

    if (m_pDevice->CreateGraphicsPipelineState(&pipelineDesc, &m_pPipelineState) != SG_OK)
        throw std::exception("Failed to create pipeline state");

    pInputLayout->Release();
}

void QueriesSample::LoadAssets()
{
    // Create blend states
    {
        // Here we use two blend states
        // The first one has blending and overwrites render target
        SG_BLEND_STATE_DESC desc{};
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcColor = SG_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestColor = SG_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = SG_BLEND_OP_ADD;

        desc.RenderTarget[0].SrcAlpha = SG_BLEND_ONE;
        desc.RenderTarget[0].DestAlpha = SG_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = SG_BLEND_OP_ADD;
        desc.RenderTarget[0].WriteMask = 0xF;

        if (m_pDevice->CreateBlendState(&desc, &m_pBlendStateRTOverride) != SG_OK)
            throw std::exception("Failed to create blend state");

        // The second has niether blending nor render target overwriting
        desc.RenderTarget[0].BlendEnable = false;
        desc.RenderTarget[0].WriteMask = 0x0;
        if (m_pDevice->CreateBlendState(&desc, &m_pBlendStateNoBlendState) != SG_OK)
            throw std::exception("Failed to create blend state");
    }

    {
        SG_DEPTH_STENCIL_STATE_DESC desc{};
        desc.DepthEnable = true;
        desc.DepthFunc = SG_COMPARISON_FUNC_LESS;
        desc.DepthWriteMask = SG_DEPTH_WRITE_MASK_ZERO;

        if (m_pDevice->CreateDepthStencilState(&desc, &m_pDepthOnlyRead) != SG_OK)
            throw std::exception("Failed to create depth stencil state");
    }

    if (m_pDevice->CreatePredicate(SG_QUERY_TYPE_BINARY_OCCLUSION, &m_pPredicate) != SG_OK)
        throw std::exception("Failed to create predicate");

    {
        SG_TEXTURE_DESC textureDesc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_DEPTH_STENCIL, m_Width, m_Height, SG_FORMAT_D16_UNORM, 1, false, false);
        textureDesc.DefaultValue.Depth = 1.0f;

        if (m_pDevice->CreateTexture(&textureDesc, &m_pDepthStencil) != SG_OK)
            throw std::exception("Failed to create depth stencil texture");

        SG_DEPTH_STENCIL_VIEW_DESC dsvDesc = FastViewDesc::AsDepthStencil(SG_FORMAT_D16_UNORM, 0, 0, 1);

        if (m_pDevice->CreateDepthStencilView(m_pDepthStencil, &dsvDesc, &m_pDSView) != SG_OK)
            throw std::exception("Failed to create depth stencil view");
    }

    for (uint32_t i = 0; i < NumFrames; i++)
    {
        SG_BUFFER_DESC cbDesc = FastBufferDesc::Constant(sizeof(SceneConstantBuffer));
        if (m_pDevice->CreateBuffer(&cbDesc, &m_pCbFarQuad[i]) != SG_OK)
            throw std::exception("Failed to create constant buffer");

        if (m_pDevice->CreateBuffer(&cbDesc, &m_pCbNearQuad[i]) != SG_OK)
            throw std::exception("Failed to create constant buffer");

        void* pData = nullptr;

        if (m_pCbFarQuad[i]->Map(&pData) != SG_OK)
            throw std::exception("Failed to map constant buffer");

        memset(pData, 0, sizeof(SceneConstantBuffer));
        m_pCbFarQuad[i]->Unmap();

        if (m_pCbNearQuad[i]->Map(&pData) != SG_OK)
            throw std::exception("Failed to map constant buffer");

        memset(pData, 0, sizeof(SceneConstantBuffer));
        m_pCbNearQuad[i]->Unmap();
    }

    // Upload frame
    m_pExecutionContext->BeginFrame();
    ISGCommandList* pCommandList;
    if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
    {
        const float m_aspectRatio = m_Width / (float)m_Height;

        Vertex quadVertices[] =
        {
            // Far quad - in practice this would be a complex geometry.
            { { -0.25f, -0.25f * m_aspectRatio, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { -0.25f, 0.25f * m_aspectRatio, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { 0.25f, -0.25f * m_aspectRatio, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
            { { 0.25f, 0.25f * m_aspectRatio, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

            // Near quad.
            { { -0.5f, -0.35f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.65f } },
            { { -0.5f, 0.35f * m_aspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.65f } },
            { { 0.5f, -0.35f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.65f } },
            { { 0.5f, 0.35f * m_aspectRatio, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.65f } },

            // Far quad bounding box used for occlusion query (offset slightly to avoid z-fighting).
            { { -0.25f, -0.25f * m_aspectRatio, 0.4999f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
            { { -0.25f, 0.25f * m_aspectRatio, 0.4999f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, -0.25f * m_aspectRatio, 0.4999f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
            { { 0.25f, 0.25f * m_aspectRatio, 0.4999f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
        };

        const UINT vertexBufferSize = sizeof(quadVertices);

        SG_BUFFER_DESC vbDesc = FastBufferDesc::Vertex(sizeof(quadVertices), true, false, false);
        m_pDevice->CreateBuffer(&vbDesc, &m_pVertexBuffer);
        UploadCommonBuffer(m_pDevice, pCommandList, m_pVertexBuffer, quadVertices, sizeof(quadVertices));

        m_pExecutionContext->FinishCommandList(pCommandList);
    }
    m_pExecutionContext->EndFrame();
}

void QueriesSample::OnUpdate()
{

}

void QueriesSample::OnRender()
{
    m_pExecutionContext->BeginFrame();

    // Update constant buffer after frame has begun to prevent data race
    {
        static SceneConstantBuffer nearCBData = {};

        static TimeScaler timeScaler;
        static float time = 0;

        timeScaler.TimeStamp();
        float timeScale = timeScaler.ComputeScale();

        const float translationSpeed = 0.01f * timeScale;
        const float offsetBounds = 1.5f;

        nearCBData.offset.x += translationSpeed;

        if (nearCBData.offset.x > offsetBounds)
        {
            nearCBData.offset.x = -offsetBounds;
        }

        UploadBuffer(m_pCbNearQuad[m_FrameIndex], &nearCBData, sizeof(nearCBData));
    }

    ISGCommandList* pCommandList = nullptr;
    if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
    {
        PrepareCommandList(pCommandList);
        m_pExecutionContext->FinishCommandList(pCommandList);
    }

    m_pExecutionContext->EndFrame1(1, &m_pSwapChain);

    m_FrameIndex = (m_FrameIndex + 1) % NumFrames;
}

void QueriesSample::PrepareCommandList(ISGCommandList* pCommandList)
{
    pCommandList->SetRenderTarget(0, m_pSwapChain->GetCurrentRTV());

    pCommandList->ClearRenderTarget(m_pSwapChain->GetCurrentRTV(), &g_ClearColor);

    pCommandList->SetDepthStencil(m_pDSView);
    pCommandList->ClearDepthStencilDefault(m_pDSView, SG_CLEAR_FLAG_DEPTH);

    // Set default depth stencil state:
    {
        //  SG_DEPTH_STENCIL_STATE_DESC desc{};
        //  desc.DepthEnable = true;
        //  desc.DepthFunc = SG_COMPARISON_FUNC_LESS;
        //  desc.DepthWriteMask = SG_DEPTH_WRITE_MASK_ALL;
        pCommandList->SetDepthStencilState(nullptr);
    }

    pCommandList->SetViewports(1, &m_Viewport);
    pCommandList->SetScissorRects(1, &m_Scissor);

    pCommandList->SetVertexBuffer(0, m_pVertexBuffer, 0, sizeof(Vertex));
    pCommandList->SetPrimitiveTopology(SG_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP);

    pCommandList->SetPipelineState(m_pPipelineState);
    pCommandList->SetBlendState(m_pBlendStateRTOverride, ~0);

    pCommandList->SetConstantBuffer(0, 0, m_pCbFarQuad[m_FrameIndex]);
    pCommandList->SetPredication(m_pPredicate, SG_PREDICATION_OP_EQUAL_ZERO);
    pCommandList->DrawInstanced(4, 1, 0, 0);

    pCommandList->SetConstantBuffer(0, 0, m_pCbNearQuad[m_FrameIndex]);
    pCommandList->SetPredication(nullptr, SG_PREDICATION_OP_EQUAL_ZERO);
    pCommandList->DrawInstanced(4, 1, 4, 0);

    pCommandList->SetBlendState(m_pBlendStateNoBlendState, ~0);
    pCommandList->SetDepthStencilState(m_pDepthOnlyRead);

    pCommandList->BeginQuery(m_pPredicate);
    pCommandList->SetConstantBuffer(0, 0, m_pCbFarQuad[m_FrameIndex]);
    pCommandList->DrawInstanced(4, 1, 8, 0);
    pCommandList->EndQuery(m_pPredicate);
}
