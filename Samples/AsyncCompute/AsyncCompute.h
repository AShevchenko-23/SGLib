
//*********************************************************
//
// Copyright (c) 2023 Aleksei Shevchenko.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "SGX/SGSample.h"
#include <DirectXMath.h>

class AsyncComputeSample : public ISGSample
{
public:
    AsyncComputeSample(U32 width, U32 height, wchar_t const* windowName);

    void OnInit() override;
    void OnDestroy() override;

    void OnUpdate() override;
    void OnRender() override;

private:
    // Number of frame buffers
    static const uint32_t NumFrames = 3;
    // Texture array size
    static const uint32_t NumSlices = 3;

    struct CommonParameters
    {
        XMMATRIX WorldTransform;
        XMMATRIX ViewProjection;
        //XMMATRIX WorldTransform;
    };

    struct JulieSetParameters
    {
        float InvTextureSizeX;
        float InvTextureSizeY;
        float Angle;
    };

    struct FrameContext
    {
        ISGBuffer* pGfxCB;
        ISGBuffer* pCmpCB;

        ISGShaderResourceView* pSRV;
        ISGUnorderedAccessView* pUAV;
    };

    SG_VIEWPORT m_Viewport;
    SG_RECT m_Scissor;

    ISGDevice* m_pDevice;
    ISGExecutionContext* m_pExecutionContext;
    ISGSwapChain* m_pSwapChain;

    ISGPipelineState* m_pGfxPipelineState;
    ISGPipelineState* m_pCmpPipelineState;

    ISGBuffer* m_pVertexBuffer;
    ISGBuffer* m_pIndexBuffer;

    ISGTexture* m_pDepthStencil;
    ISGDepthStencilView* m_pDSView;
    ISGSampler* m_pSampler;

    ISGBuffer* m_pCbsGfx[NumFrames];
    ISGBuffer* m_pCbsCmp[NumFrames];

    ISGTexture* m_pTexture;
    ISGShaderResourceView* m_pTextureSRVs[NumSlices];
    ISGUnorderedAccessView* m_pTextureUAVs[NumSlices];

    Camera m_Camera;
    TimeScaler m_TimeScaler;
    uint32_t m_FrameIndex;
    float m_CurrentAngle;

    FrameContext m_FrameContext;

    static_assert(NumSlices >= 2, "Number of texture slices must be greater or equal 2");
    static_assert(_countof(m_pCbsGfx) == NumFrames, "Number of constant buffers for the graphics pass must be equal NumFrames");
    static_assert(_countof(m_pCbsCmp) == NumFrames, "Number of constant buffers for the compute pass must be equal NumFrames");
    static_assert(_countof(m_pTextureSRVs) == NumSlices, "Number of texture SRVs must be equal NumSlices");
    static_assert(_countof(m_pTextureUAVs) == NumSlices, "Number of texture UAVs must be equal NumSlices");

    void LoadPipelineStates();
    void LoadAssets();

    void PrepareGraphicsCommandList(ISGCommandList* pCommandList, FrameContext* pFrameCtx);
    void PrepareComputeCommandList(ISGCommandList* pCommandList, FrameContext* pFrameCtx);
};
