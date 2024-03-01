
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

#include "RaytracingSample.h"
#include "SGX/Box.h"

RaytracingSample::RaytracingSample(U32 width, U32 height, wchar_t const* windowName)
    : ISGSample(width, height, windowName)
    , m_pDevice(SG_NULL)
    , m_pExecutionContext(SG_NULL)
    , m_pSwapChain(SG_NULL)

    , m_pPipelineState(SG_NULL)

    , m_pVertexBuffer(SG_NULL)
    , m_pVertexBufferSRV(SG_NULL)

    , m_pIndexBuffer(SG_NULL)
    , m_pIndexBufferSRV(SG_NULL)

    , m_pConstantBuffer(SG_NULL)
    , m_pTransformBuffers{}

    , m_pOutTexture(SG_NULL)
    , m_pOutTextureUAV(SG_NULL)

    , m_pSimpleTexture(SG_NULL)
    , m_pSimpleTextureSRV(SG_NULL)
    , m_pAlphaTestTexture(SG_NULL)
    , m_pAlphaTestTextureSRV(SG_NULL)

    , m_pSampler(SG_NULL)

    , m_pFloorBLAS(SG_NULL)
    , m_pSceneBVH(SG_NULL)

    , m_Boxes{}
    , m_FrameIndex(0)
    , m_Time(0.0f)
{
}

void RaytracingSample::OnInit()
{
    ISGAdapter* pAdapter = nullptr;
    if (GetVideoAdapter(&pAdapter) != SG_OK)
        throw std::exception("Compatible adapter was not found");

    SG_ADAPTER_INFO_EXECUTION info;
    pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_EXECUTION, &info);

    if (info.RaytracingTier == SG_RAYTRACING_TIER_NOT_SUPPORTED)
        throw std::exception("Adapter doesn't support ray tracing");

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

void RaytracingSample::OnDestroy()
{
    m_pExecutionContext->WaitForIdle();

    SG_RELEASE(m_pSceneBVH);

    for (int i = 0; i < _countof(m_Boxes); i++)
        SG_RELEASE(m_Boxes[i].pBLAS);

    SG_RELEASE(m_pFloorBLAS);

    for (U32 i = 0; i < _countof(m_pTransformBuffers); i++)
        SG_RELEASE(m_pTransformBuffers[i]);

    SG_RELEASE(m_pIndexBufferSRV);
    SG_RELEASE(m_pIndexBuffer);

    SG_RELEASE(m_pVertexBufferSRV);
    SG_RELEASE(m_pVertexBuffer);

    SG_RELEASE(m_pSampler);

    SG_RELEASE(m_pConstantBuffer);

    SG_RELEASE(m_pPipelineState);

    SG_RELEASE(m_pAlphaTestTextureSRV);
    SG_RELEASE(m_pAlphaTestTexture);

    SG_RELEASE(m_pSimpleTextureSRV);
    SG_RELEASE(m_pSimpleTexture);

    SG_RELEASE(m_pOutTextureUAV);
    SG_RELEASE(m_pOutTexture);

    SG_RELEASE(m_pSwapChain);
    SG_RELEASE(m_pDevice);
}

void RaytracingSample::LoadPipelineState()
{
    ByteBuffer rtLib;
    if (!LoadBinaryFile("RaytracingLib.cso", rtLib))
        throw std::exception("Failed to load shader library");

    SG_RAYTRACING_PIPELINE_DESC pipelineDesc{};

    pipelineDesc.RayGenShader.pName = "RayGen";
    pipelineDesc.RayGenShader.ByteCode = { rtLib.data(), rtLib.size() };

    pipelineDesc.MissShader.pName = "Miss";
    pipelineDesc.MissShader.ByteCode = { rtLib.data(), rtLib.size() };

    SG_RAYTRACING_HIT_GROUP hitGroups[4];
    memset(hitGroups, 0, sizeof(hitGroups));

    hitGroups[0].Type = SG_HIT_GROUP_TYPE_TRIANGLES;
    hitGroups[0].ClosestHit.pName = "ClosestHitFloor";
    hitGroups[0].ClosestHit.ByteCode = { rtLib.data(), rtLib.size() };

    hitGroups[1].Type = SG_HIT_GROUP_TYPE_TRIANGLES;
    hitGroups[1].ClosestHit.pName = "ClosestHitBary";
    hitGroups[1].ClosestHit.ByteCode = { rtLib.data(), rtLib.size() };

    hitGroups[2].Type = SG_HIT_GROUP_TYPE_TRIANGLES;
    hitGroups[2].ClosestHit.pName = "ClosestHitSimpleTexture";
    hitGroups[2].ClosestHit.ByteCode = { rtLib.data(), rtLib.size() };

    hitGroups[3].Type = SG_HIT_GROUP_TYPE_TRIANGLES;
    hitGroups[3].AnyHit.pName = "AnyHitAlphaTest";
    hitGroups[3].AnyHit.ByteCode = { rtLib.data(), rtLib.size() };

    pipelineDesc.HitGroupCount = _countof(hitGroups);
    pipelineDesc.pHitGroups = hitGroups;

    pipelineDesc.MaxPayloadSize = sizeof(float) * 4;   // size of PayloadData in shader
    pipelineDesc.MaxAttributeSize = sizeof(float) * 2; // size of BuiltInTriangleIntersectionAttributes in shader
    pipelineDesc.MaxRecursionDepth = 2;                // First ray + reflection

    SG_BINDING_TABLE_DESC table{};

    {
        table.ShaderVisibility = SG_SHADER_VISIBILITY_ALL;
        table.ConstantBuffers   = { 0, 0, 1 };  // RayGenConstantBuffer
        table.SRVs              = { 0, 0, 4 };  // Vertex and index structure buffers; Simple and Alpha-test textures
        table.AsSRVs            = { 0, 4, 1 };  // Acceleration structure views are SRVs in fact, they must not overlap regular SRVs ranges
        table.UAVs              = { 0, 0, 1 };  // OutTexture
        table.Samplers          = { 0, 0, 1 };  // Sampler
    }

    pipelineDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
    pipelineDesc.RootSignature.Tabular.NumTables = 1;
    pipelineDesc.RootSignature.Tabular.pTables = &table;

    if (m_pDevice->CreateRaytracingPipelineState(&pipelineDesc, &m_pPipelineState) != SG_OK)
        throw std::exception("Failed to create pipeline state object");
}

void RaytracingSample::LoadAssets()
{
    // Create and upload constant buffer
    {
        // Here we won't update camera parameters, so no need to have several constant buffers for each frame.
        Camera camera(true);
        camera.SetTarget({ 0.0f, 0.0f, 0.0f });
        camera.SetPosition({ 0.0f, 5.0f, -10.0f });
        camera.SetFOV(DEG2RAD(45));
        camera.SetAspectRatio(m_Width / (float)m_Height);
        camera.SetZRange(1.0f, 150.0f);
        camera.Update();

        struct RayGenConstants
        {
            XMMATRIX ProjToWorldMatrix;
            XMVECTOR CameraPosition;
        } cameraParams;

        XMMATRIX viewProjection = camera.GetViewProjection();
        cameraParams.ProjToWorldMatrix = XMMatrixInverse(nullptr, viewProjection);
        cameraParams.CameraPosition = { 0.0f, 5.0f, -10.0f };

        SG_BUFFER_DESC cbDesc = FastBufferDesc::Constant(sizeof(cameraParams));

        if (m_pDevice->CreateBuffer(&cbDesc, &m_pConstantBuffer) != SG_OK)
            throw std::exception("Failed to create constant buffer");

        UploadBuffer(m_pConstantBuffer, &cameraParams, sizeof(cameraParams));
    }

    // Create out texture
    {
        SG_TEXTURE_DESC desc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_COMMON, m_Width, m_Height, SG_FORMAT_R8G8B8A8_UNORM, 1, true, true);
        if (m_pDevice->CreateTexture(&desc, &m_pOutTexture) != SG_OK)
            throw std::exception("Failed to create texture");

        SG_UNORDERED_ACCESS_VIEW_DESC uavDesc = FastViewDesc::AsRWTexture(SG_FORMAT_R8G8B8A8_UNORM, 0, 0, 0);
        if (m_pDevice->CreateUnorderedAccessView(m_pOutTexture, &uavDesc, &m_pOutTextureUAV) != SG_OK)
            throw std::exception("Failed to create texture UAV");
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

    // Set-up boxes parameters
    {
        m_Boxes[0].BasePosition = XMFLOAT3(-3.0f, 0.0f, 0.0f);
        m_Boxes[0].HitGroupIndex = 1;
        m_Boxes[0].AlphaTest = false;
        m_Boxes[0].pBLAS = SG_NULL;

        m_Boxes[1].BasePosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
        m_Boxes[1].HitGroupIndex = 2;
        m_Boxes[1].AlphaTest = false;
        m_Boxes[1].pBLAS = SG_NULL;

        m_Boxes[2].BasePosition = XMFLOAT3(3.0f, 0.0f, 0.0f);
        m_Boxes[2].HitGroupIndex = 3;
        m_Boxes[2].AlphaTest = true;
        m_Boxes[2].pBLAS = SG_NULL;
    }

    // Create a buffer with local transforms for each dynamic box
    {
        SG_BUFFER_DESC desc = FastBufferDesc::Upload(sizeof(XMMATRIX) * _countof(m_Boxes));

        for (U32 i = 0; i < _countof(m_pTransformBuffers); i++)
        {
            if (m_pDevice->CreateBuffer(&desc, &m_pTransformBuffers[i]) != SG_OK)
                throw std::exception("Failed to create transform buffer");
        }
    }

    ISGTexture* pUploadTexture1 = SG_NULL;
    ISGTexture* pUploadTexture2 = SG_NULL;

    {
        ImageDesc imageDesc;
        ByteBuffer bitmap;

        // Load and create simple texture
        {
            if (!LoadTGA("SimpleImage.tga", imageDesc, bitmap))
                throw std::exception("Failed to load image");

            if (!CreateUploadTextureFromData(m_pDevice, imageDesc, bitmap, &pUploadTexture1))
                throw std::exception("Failed to create upload texture");

            SG_TEXTURE_DESC desc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_COMMON, imageDesc.Width, imageDesc.Height, imageDesc.Format, 1, true, false);
            if (m_pDevice->CreateTexture(&desc, &m_pSimpleTexture) != SG_OK)
                throw std::exception("Failed to create texture");

            SG_SHADER_RESOURCE_VIEW_DESC viewDesc = FastViewDesc::AsTexture(imageDesc.Format, 0, 1, 0, 0);
            if (m_pDevice->CreateShaderResourceView(m_pSimpleTexture, &viewDesc, &m_pSimpleTextureSRV) != SG_OK)
                throw std::exception("Failed to create texture SRV");
        }

        // Load and create alpha test texture
        {
            if (!LoadTGA("AlphaTest.tga", imageDesc, bitmap))
                throw std::exception("Failed to load image");

            if (!CreateUploadTextureFromData(m_pDevice, imageDesc, bitmap, &pUploadTexture2))
                throw std::exception("Failed to create upload texture");

            SG_TEXTURE_DESC desc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_COMMON, imageDesc.Width, imageDesc.Height, imageDesc.Format, 1, true, false);
            if (m_pDevice->CreateTexture(&desc, &m_pAlphaTestTexture) != SG_OK)
                throw std::exception("Failed to create texture");

            SG_SHADER_RESOURCE_VIEW_DESC viewDesc = FastViewDesc::AsTexture(imageDesc.Format, 0, 1, 0, 0);
            if (m_pDevice->CreateShaderResourceView(m_pAlphaTestTexture, &viewDesc, &m_pAlphaTestTextureSRV) != SG_OK)
                throw std::exception("Failed to create texture SRV");
        }
    }

    // Create vertex buffer and its SRV
    {
        SG_BUFFER_DESC vbDesc = FastBufferDesc::Vertex(sizeof(g_pBoxVertices), true, false, false);
        if (m_pDevice->CreateBuffer(&vbDesc, &m_pVertexBuffer) != SG_OK)
            throw std::exception("Failed to create vertex buffer");

        SG_SHADER_RESOURCE_VIEW_DESC vbSrvDesc = FastViewDesc::AsStructuredBuffer(0, _countof(g_pBoxVertices), sizeof(Vertex));
        if (m_pDevice->CreateShaderResourceView(m_pVertexBuffer, &vbSrvDesc, &m_pVertexBufferSRV) != SG_OK)
            throw std::exception("Failed to create vertex buffer SRV");
    }

    // Create index buffer and its SRV
    {
        SG_BUFFER_DESC ibDesc = FastBufferDesc::Index(sizeof(g_pBoxIndices), true, false, false);
        if (m_pDevice->CreateBuffer(&ibDesc, &m_pIndexBuffer) != SG_OK)
            throw std::exception("Failed to create index buffer");

        SG_SHADER_RESOURCE_VIEW_DESC ibSrvDesc = FastViewDesc::AsStructuredBuffer(0, _countof(g_pBoxIndices), sizeof(U16));
        if (m_pDevice->CreateShaderResourceView(m_pIndexBuffer, &ibSrvDesc, &m_pIndexBufferSRV) != SG_OK)
            throw std::exception("Failed to create index buffer SRV");
    }

    // Create bottom level acceleration structure of the floor box
    {
        SG_AS_GEOMETRY_DESC geometryDesc{};
        geometryDesc.Type = SG_AS_GEOMETRY_TYPE_TRIANGLES;
        geometryDesc.Flags = SG_AS_GEOMETRY_FLAG_OPAQUE;

        // Set-up index buffer.
        geometryDesc.Triangles.IndexBuffer = { m_pIndexBuffer, 0 };
        geometryDesc.Triangles.IndexCount = _countof(g_pBoxIndices);
        geometryDesc.Triangles.IndexFormat = SG_FORMAT_R16_UINT;

        // Set-up vertex buffer.
        geometryDesc.Triangles.VertexBuffer = { m_pVertexBuffer, 0, sizeof(Vertex) };
        geometryDesc.Triangles.VertexFormat = SG_FORMAT_R32G32B32_FLOAT;
        geometryDesc.Triangles.VertexCount = _countof(g_pBoxVertices);

        // No local transforms
        geometryDesc.Triangles.Transform3x4 = { nullptr, 0 };

        SG_AS_BOTTOM_LEVEL_DESC blasDesc{};
        blasDesc.Preferences = SG_AS_BUILD_PREFERENCE_FAST_TRACE;
        blasDesc.NumDescs = 1;
        blasDesc.pGeometryDescs = &geometryDesc;

        if (m_pDevice->CreateBottomLevelAccelerationStructure(&blasDesc, &m_pFloorBLAS) != SG_OK)
            throw std::exception("Failed to create acceleration structure");
    }

    // Upload frame
    {
        m_pExecutionContext->BeginFrame();

        ISGCommandList* pCommandList;
        if (m_pExecutionContext->ScheduleCommandList(0, 1, &pCommandList) == SG_OK)
        {
            {
                UploadCommonBuffer(m_pDevice, pCommandList, m_pVertexBuffer, g_pBoxVertices, sizeof(g_pBoxVertices));
                UploadCommonBuffer(m_pDevice, pCommandList, m_pIndexBuffer, g_pBoxIndices, sizeof(g_pBoxIndices));
            }

            {
                pCommandList->CopyResource(m_pSimpleTexture, pUploadTexture1);
                pCommandList->CopyResource(m_pAlphaTestTexture, pUploadTexture2);
            }

            {
                pCommandList->BuildBottomLevelAS(m_pFloorBLAS);
            }

            m_pExecutionContext->FinishCommandList(pCommandList);
        }
        m_pExecutionContext->EndFrame();
    }

    pUploadTexture1->Release();
    pUploadTexture2->Release();
}

void RaytracingSample::OnUpdate()
{
    // Calculate animation time
    {
        const float speed = DEG2RAD(3.0f);

        m_TimeScaler.TimeStamp();
        m_Time += speed * m_TimeScaler.ComputeScale();

        if (m_Time > XM_2PI)
            m_Time -= XM_2PI;
    }

    SG_AS_INSTANCE_DESC instances[4];

    // Update dynamic boxes transformation and prepare their BLAS building info.
    for (int i = 0; i < NumBoxes; i++)
    {
        Box& box = m_Boxes[i];
        float boxTime = m_Time + XM_PI / 3.0f * i;

        // Prepare bottom level acceleration structure of dynamic box.
        {
            SG_AS_GEOMETRY_DESC geometryDesc{};
            geometryDesc.Type = SG_AS_GEOMETRY_TYPE_TRIANGLES;

            // Set SG_AS_GEOMETRY_FLAG_OPAQUE for opaqueu boxes to optimize tracing
            geometryDesc.Flags = box.AlphaTest ? SG_AS_GEOMETRY_FLAG_NONE : SG_AS_GEOMETRY_FLAG_OPAQUE;

            // Set-up index buffer.
            geometryDesc.Triangles.IndexBuffer = { m_pIndexBuffer, 0 };
            geometryDesc.Triangles.IndexCount = _countof(g_pBoxIndices);
            geometryDesc.Triangles.IndexFormat = SG_FORMAT_R16_UINT;

            // Set-up vertex buffer.
            geometryDesc.Triangles.VertexBuffer = { m_pVertexBuffer, 0, sizeof(Vertex) };
            geometryDesc.Triangles.VertexFormat = SG_FORMAT_R32G32B32_FLOAT;
            geometryDesc.Triangles.VertexCount = _countof(g_pBoxVertices);

            // Set-up transform parameters using common buffer for all dynamic boxes.
            {
                // Select a transform buffer for the current frame
                ISGBuffer* pTransformBuffer = m_pTransformBuffers[m_FrameIndex];

                // At first just set buffer and offset
                geometryDesc.Triangles.Transform3x4.pBuffer     = pTransformBuffer;
                geometryDesc.Triangles.Transform3x4.ByteOffset  = i * sizeof(Matrix3x4);

                // And calculate a matrix.
                // It will be uploaded before building when frame has begun to prevent data race.
                CalculateGeometryTransform(boxTime, &m_TransformData[i]);
            }

            // Create bottom level acceleration structure from scratch, if it has not been done yet.
            if (box.pBLAS == nullptr)
            {
                SG_AS_BOTTOM_LEVEL_DESC blasDesc{};
                blasDesc.Flags = SG_AS_BUILD_FLAG_ALLOW_UPDATE;      // Allow BLAS to fast update
                blasDesc.Preferences = SG_AS_BUILD_PREFERENCE_NONE;
                blasDesc.NumDescs = 1;
                blasDesc.pGeometryDescs = &geometryDesc;

                m_pDevice->CreateBottomLevelAccelerationStructure(&blasDesc, &box.pBLAS);
            }
            else // Otherwise just refresh geometry descriptions and request an updating present BLAS.
            {
                box.pBLAS->Fill(1, &geometryDesc, true, nullptr);
            }
        }

        // Prepare instance description of dynamic box.
        {
            SG_AS_INSTANCE_DESC& instance = instances[i + 1];

            CalculateInstanceTransform(boxTime, box.BasePosition, (Matrix3x4*)(instance.Transform));
            instance.InstanceMask = 0xFF;
            instance.InstanceID = 1;
            instance.InstanceContributionToHitGroupIndex = box.HitGroupIndex;
            instance.Flags = 0;
            instance.pAccelerationStructure = box.pBLAS;
        }
    }

    // Prepare an description of the floor instance.
    {
        float floorSize = 10.0f;
        float floorHeight = 0.1f;
        XMMATRIX floorTransform = XMMatrixScaling(floorSize, floorHeight, floorSize);
        floorTransform = XMMatrixMultiply(floorTransform, XMMatrixTranslation(0, -0.1f, 0));
        floorTransform = XMMatrixTranspose(floorTransform);

        SG_AS_INSTANCE_DESC& instance = instances[0];

        memcpy(instance.Transform, (float const*)&floorTransform, sizeof(float) * 12);
        instance.InstanceMask = 0xFF;
        instance.InstanceID = 1;
        instance.InstanceContributionToHitGroupIndex = 0;
        instance.Flags = 0;
        instance.pAccelerationStructure = m_pFloorBLAS;
    }

    // Create top level acceleration structure from scratch, if it has not been done yet.
    if (m_pSceneBVH == nullptr)
    {
        SG_AS_TOP_LEVEL_DESC  topLevelDesc;
        topLevelDesc.Flags = SG_AS_BUILD_FLAG_NONE;
        topLevelDesc.Preferences = SG_AS_BUILD_PREFERENCE_NONE;
        topLevelDesc.NumDescs = _countof(instances);
        topLevelDesc.pInstanceDescs = instances;

        m_pDevice->CreateTopLevelAccelerationStructure(&topLevelDesc, &m_pSceneBVH);
    }
    else // Otherwise just refresh instance descriptions and request for full rebuild.
    {
        m_pSceneBVH->Fill(_countof(instances), instances, false, nullptr);
    }
}

void RaytracingSample::OnRender()
{
    m_pExecutionContext->BeginFrame();

    // Update transform buffer after frame has begun to prevent data race
    {
        // Map the buffer data to set local transforms
        void* pData = nullptr;
        if (m_pTransformBuffers[m_FrameIndex]->Map(&pData) != SG_OK)
            throw std::exception("Failed to map buffer data");

        memcpy(pData, m_TransformData, sizeof(m_TransformData));

        m_pTransformBuffers[m_FrameIndex]->Unmap();
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

void RaytracingSample::CalculateGeometryTransform(float time, Matrix3x4* pGeometryTransform)
{
    float scaleInterpolation = max(0.0f, cosf(time * 2) - 0.7f);

    const float hScaleMin = 1.0f;
    const float hScaleMax = 1.8f;
    float hScale = hScaleMin + (hScaleMax - hScaleMin) * scaleInterpolation;

    const float vScaleMin = 1.0f;
    const float vScaleMax = 0.8f;
    float vScale = vScaleMin + (vScaleMax - vScaleMin) * scaleInterpolation;

    XMMATRIX geometryTransform;

    // Set origin of the box at its bottom
    geometryTransform = XMMatrixTranslation(0, 1, 0);
    // Scale box according to the animation period
    geometryTransform = XMMatrixMultiply(geometryTransform, XMMatrixScaling(hScale, vScale, hScale));
    // Matrix should be in row-major layout
    geometryTransform = XMMatrixTranspose(geometryTransform);

    memcpy(pGeometryTransform, &geometryTransform, sizeof(*pGeometryTransform));
}

void RaytracingSample::CalculateInstanceTransform(float time, XMFLOAT3 const& basePosition, Matrix3x4* pInstanceTransform)
{
    const float jumpMinHeight = 0.0f;
    const float jumpMaxHeight = 2.0f;
    float jumpInterpolation = max(0, abs(sin(time)) - 0.3f);

    float jumpHeight = jumpMinHeight + (jumpMaxHeight - jumpMinHeight) * jumpInterpolation;

    XMMATRIX instanceTransform;

    // Set position of box
    instanceTransform = XMMatrixTranslation(basePosition.x, jumpHeight, basePosition.z);
    // Matrix should be in row-major layout
    instanceTransform = XMMatrixTranspose(instanceTransform);

    memcpy(pInstanceTransform, &instanceTransform, sizeof(*pInstanceTransform));
}

void RaytracingSample::PrepareCommandList(ISGCommandList* pCommandList)
{
    // Build acceleration structures.
    {
        for (U32 i = 0; i < _countof(m_Boxes); i++)
            pCommandList->BuildBottomLevelAS(m_Boxes[i].pBLAS);
        pCommandList->BuildTopLevelAS(m_pSceneBVH);
    }

    // Trace rays
    {
        pCommandList->SetPipelineState(m_pPipelineState);

        pCommandList->SetConstantBuffer(0, 0, m_pConstantBuffer);

        pCommandList->SetShaderResource(0, 0, m_pVertexBufferSRV);
        pCommandList->SetShaderResource(0, 1, m_pIndexBufferSRV);
        pCommandList->SetShaderResource(0, 2, m_pSimpleTextureSRV);
        pCommandList->SetShaderResource(0, 3, m_pAlphaTestTextureSRV);

        pCommandList->SetAccelerationStructure(0, 0, m_pSceneBVH);

        pCommandList->SetUnorderedAccessView(0, 0, m_pOutTextureUAV);

        pCommandList->SetSampler(0, 0, m_pSampler);

        pCommandList->DispatchRays(m_Width, m_Height, 2);
    }

    // Copy results to a back buffer
    pCommandList->CopyResource(m_pSwapChain->GetCurrentTexture(), m_pOutTexture);
}
