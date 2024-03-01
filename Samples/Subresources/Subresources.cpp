
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

#include "Subresources.h"
#include "SGX/Box.h"

const SG_COLOR_4F g_ClearColor = { 0.8f, 0.9f, 1.0f, 1.0f };

const char const* g_pTexFN = "Mips.tga";
const uint32_t g_TexSize = 128;
const uint32_t g_BoxFaces = 6;
const uint32_t g_MipLevels = 3;

Subresources::Subresources(U32 width, U32 height, wchar_t const* windowName)
    : ISGSample(width, height, windowName)
    , m_Viewport{ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f }
    , m_Scissor{ 0, 0, static_cast<int>(width), static_cast<int>(height) }

    , m_pDevice(SG_NULL)
    , m_pExecutionContext(SG_NULL)
    , m_pSwapChain(SG_NULL)

    , m_pPipelineState(SG_NULL)

    , m_pVertexBuffer(SG_NULL)
    , m_pIndexBuffer(SG_NULL)

    , m_pDepthStencil(SG_NULL)
    , m_pDSView(SG_NULL)

    , m_pSampler(SG_NULL)

    , m_pTexture(SG_NULL)
    , m_pTextureSRV(SG_NULL)

    , m_pConstantBuffers{}

    , m_Camera(true)
    , m_CurrentAngle(0.0f)
    , m_FrameIndex(0)
{
    m_Camera.SetFOV(DEG2RAD(45));
    m_Camera.SetAspectRatio(m_Width / static_cast<float>(m_Height));
    m_Camera.SetZRange(1.0f, 25.0f);
    m_Camera.SetTarget({ 0.0f, 0.0f, 0.0f });
}

void Subresources::OnInit()
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

void Subresources::OnDestroy()
{
    m_pExecutionContext->WaitForIdle();

    SG_RELEASE(m_pTextureSRV);
    SG_RELEASE(m_pTexture);

    for (uint32_t i = 0; i < _countof(m_pConstantBuffers); i++)
        SG_RELEASE(m_pConstantBuffers[i]);

    SG_RELEASE(m_pIndexBuffer);
    SG_RELEASE(m_pVertexBuffer);

    SG_RELEASE(m_pDSView);
    SG_RELEASE(m_pDepthStencil);

    SG_RELEASE(m_pSampler);

    SG_RELEASE(m_pPipelineState);

    SG_RELEASE(m_pSwapChain);
    SG_RELEASE(m_pExecutionContext);
    SG_RELEASE(m_pDevice);
}

void Subresources::LoadPipelineState()
{
    SG_INPUT_ELEMENT_DESC inputElements[] =
    {
        { "POSITION", 0, SG_FORMAT_R32G32B32_FLOAT, 0, 0, SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, SG_FORMAT_R32G32_FLOAT, 0, 12, SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    ISGInputLayout* pInputLayout = nullptr;
    if (m_pDevice->CreateInputLayout(_countof(inputElements), inputElements, &pInputLayout) != SG_OK)
        throw std::exception("Failed to create input layout");

    bool success = true;

    ByteBuffer vsBuffer, psBuffer;
    if (!LoadBinaryFile("VertexShader.cso", vsBuffer))
        throw std::exception("Failed to load vertex shader");

    if (!LoadBinaryFile("PixelShader.cso", psBuffer))
        throw std::exception("Failed to load pixel shader");

    if (success)
    {
        SG_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
        pipelineDesc.pInputLayout = pInputLayout;
        pipelineDesc.VS = { vsBuffer.data(), vsBuffer.size() };
        pipelineDesc.PS = { psBuffer.data(), psBuffer.size() };

        SG_BINDING_TABLE_DESC tableDesc{};
        tableDesc.ConstantBuffers = { 0, 0, 1 };
        tableDesc.SRVs = { 0, 0, 1 };
        tableDesc.Samplers = { 0, 0, 1 };
        tableDesc.ShaderVisibility = SG_SHADER_VISIBILITY_ALL;

        pipelineDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
        pipelineDesc.RootSignature.Tabular.NumTables = 1;
        pipelineDesc.RootSignature.Tabular.pTables = &tableDesc;

        if (m_pDevice->CreateGraphicsPipelineState(&pipelineDesc, &m_pPipelineState) != SG_OK)
            throw std::exception("Failed to create pipeline state");
    }

    pInputLayout->Release();
}

void Subresources::LoadAssets()
{
    // Create depth stencil
    {
        SG_TEXTURE_DESC depthBufferDesc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_DEPTH_STENCIL, m_Width, m_Height, SG_FORMAT_D16_UNORM, 1, false, false);
        depthBufferDesc.DefaultValue.Format = SG_FORMAT_D16_UNORM;
        depthBufferDesc.DefaultValue.Depth = 1.0f;

        if (m_pDevice->CreateTexture(&depthBufferDesc, &m_pDepthStencil) != SG_OK)
            throw std::exception("Failed to create depth texture");

        SG_DEPTH_STENCIL_VIEW_DESC dsvDesc = FastViewDesc::AsDepthStencil(SG_FORMAT_D16_UNORM, 0, 0);

        if (m_pDevice->CreateDepthStencilView(m_pDepthStencil, &dsvDesc, &m_pDSView) != SG_OK)
            throw std::exception("Failed to create depth stencil view");
    }

    // Create sampler
    {
        // SG_SAMPLER_DESC samplerDesc = FastSamplerDesc::Simple(SG_TEXTURE_ADDRESS_MODE_CLAMP, SG_FILTER_TYPE_LINEAR, 0);

        SG_SAMPLER_DESC samplerDesc = {};
        samplerDesc.AddressU = SG_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.AddressV = SG_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.AddressW = SG_TEXTURE_ADDRESS_MODE_CLAMP;
        samplerDesc.Filter = SG_ENCODE_BASIC_FILTER(SG_FILTER_TYPE_LINEAR, SG_FILTER_TYPE_LINEAR, SG_FILTER_TYPE_LINEAR, SG_FILTER_REDUCTION_STANDART);
        samplerDesc.MaxLOD = g_MipLevels;

        if (m_pDevice->CreateSampler(&samplerDesc, &m_pSampler) != SG_OK)
            throw std::exception("Failed to create sampler");
    }

    // Create sets of constant buffers for all frames to prevent data race
    for (uint32_t i = 0; i < NumFrames; i++)
    {
        SG_BUFFER_DESC cbDesc = FastBufferDesc::Constant(uint32_t(sizeof(XMMATRIX)));

        if (m_pDevice->CreateBuffer(&cbDesc, &m_pConstantBuffers[i]) != SG_OK)
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

    ImageDesc imageDesc = {};
    ISGTexture* pUploadTexture = SG_NULL;

    // Create intermediate texture from a loaded image
    {
        ByteBuffer bitmap;
        if (!LoadTGA(g_pTexFN, imageDesc, bitmap))
            throw std::exception("Failed to load image");

        CreateUploadTextureFromData(m_pDevice, imageDesc, bitmap, &pUploadTexture);
    }

    // Create a target texture and its view
    {
        SG_TEXTURE_DESC texDesc = FastTextureDesc::Tex2DArray(SG_TEXTURE_TYPE_COMMON, g_TexSize, g_TexSize, imageDesc.Format, g_MipLevels, g_BoxFaces, true, false, false);

        if (m_pDevice->CreateTexture(&texDesc, &m_pTexture) != SG_OK)
            throw std::exception("Failed to create texture");

        SG_SHADER_RESOURCE_VIEW_DESC viewDesc = FastViewDesc::AsTextureArray(imageDesc.Format, 0, g_MipLevels, 0, g_BoxFaces, 0);
        if (m_pDevice->CreateShaderResourceView(m_pTexture, &viewDesc, &m_pTextureSRV) != SG_OK)
            throw std::exception("Failed to create texture SRV");
    }

    // Dummy frame to upload resources
    {
        m_pExecutionContext->BeginFrame();
        ISGCommandList* pCommandList;
        if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
        {
            // Upload vertex and index buffers
            UploadCommonBuffer(m_pDevice, pCommandList, m_pVertexBuffer, g_pBoxVertices, sizeof(g_pBoxVertices));
            UploadCommonBuffer(m_pDevice, pCommandList, m_pIndexBuffer, g_pBoxIndices, sizeof(g_pBoxIndices));

            // Fill the texture in parts
            {
                for (uint32_t arraySlice = 0; arraySlice < g_BoxFaces; arraySlice++)
                {
                    uint32_t leftOffset = g_TexSize * arraySlice;

                    for (uint32_t mip = 0; mip < g_MipLevels; mip++)
                    {
                        const uint32_t mipSize = g_TexSize >> mip;
                        const uint32_t topOffset = 2 * (g_TexSize - mipSize);

                        SG_TEXTURE_COPY_DESTINATION dest;
                        dest.Dimension = SG_TEXTURE_DIMENSION_2D;
                        dest.Tex2D.ArraySlice = arraySlice;
                        dest.Tex2D.Mip = mip;
                        dest.Tex2D.Left = 0;
                        dest.Tex2D.Top = 0;

                        SG_TEXTURE_COPY_SOURCE source;
                        source.Dimension = SG_TEXTURE_DIMENSION_2D;
                        source.Tex2D.ArraySlice = 0;
                        source.Tex2D.Mip = 0;
                        source.Tex2D.Left = leftOffset;
                        source.Tex2D.Top = topOffset;
                        source.Tex2D.Width = mipSize;
                        source.Tex2D.Height = mipSize;

                        pCommandList->CopyTextureRegion(m_pTexture, &dest, pUploadTexture, &source);
                    }
                }

                // Pipeline captures commited resources while they're processed, after all they will be released
                // Here we've done with pUploadTexture and it needs to be released on our side
                pUploadTexture->Release();
            }

            m_pExecutionContext->FinishCommandList(pCommandList);
        }

        m_pExecutionContext->EndFrame();
    }
}

void Subresources::OnUpdate()
{
    // Calculate current rotating angle of the box
    const float rotationSpeed = DEG2RAD(0.5f);

    m_TimeScaler.TimeStamp();
    m_CurrentAngle += rotationSpeed * m_TimeScaler.ComputeScale();

    if (m_CurrentAngle > XM_2PI)
        m_CurrentAngle -= XM_2PI;

    // Calculate camera position
    {
        const float CamDistance = 7.5f;

        float camX = cosf(m_CurrentAngle);
        float camY = cosf(m_CurrentAngle);
        float camZ = sinf(m_CurrentAngle);

        float scale = CamDistance / sqrt(camX * camX + camY * camY + camZ * camZ);

        camX *= scale;
        camY *= scale;
        camZ *= scale;

        m_Camera.SetPosition({ camX, camY, camZ });
    }

    m_Camera.Update();
}

void Subresources::OnRender()
{
    m_pExecutionContext->BeginFrame();

    // Update constant buffer after frame has begun to prevent data race
    {
        XMMATRIX viewProjMatrix = m_Camera.GetViewProjection();
        UploadBuffer(m_pConstantBuffers[m_FrameIndex], &viewProjMatrix, sizeof(viewProjMatrix));
    }

    ISGCommandList* pCommandList;
    if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
    {
        PrepareCommandList(pCommandList);
    }

    m_pExecutionContext->EndFrame1(1, &m_pSwapChain);
    m_FrameIndex = (m_FrameIndex + 1) % NumFrames;
}

void Subresources::PrepareCommandList(ISGCommandList* pCommandList)
{
    const SG_COLOR_4F SkyColor = { 0.8f, 0.9f, 1.0f, 1 };

    pCommandList->SetRenderTarget(0, m_pSwapChain->GetCurrentRTV());
    pCommandList->ClearRenderTarget(m_pSwapChain->GetCurrentRTV(), &g_ClearColor);

    pCommandList->SetDepthStencil(m_pDSView);
    pCommandList->ClearDepthStencilDefault(m_pDSView, SG_CLEAR_FLAG_DEPTH);

    pCommandList->SetViewports(1, &m_Viewport);
    pCommandList->SetScissorRects(1, &m_Scissor);

    pCommandList->SetVertexBuffer(0, m_pVertexBuffer, 0, sizeof(Vertex));
    pCommandList->SetIndexBuffer(m_pIndexBuffer, 0, SG_FORMAT_R16_UINT);

    pCommandList->SetPipelineState(m_pPipelineState);
    pCommandList->SetPrimitiveTopology(SG_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    pCommandList->SetConstantBuffer(0, 0, m_pConstantBuffers[m_FrameIndex]);
    pCommandList->SetShaderResource(0, 0, m_pTextureSRV);
    pCommandList->SetSampler(0, 0, m_pSampler);

    pCommandList->DrawIndexedInstanced(_countof(g_pBoxIndices), 1, 0, 0, 0);

    m_pExecutionContext->FinishCommandList(pCommandList);
}
