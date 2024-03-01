
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

#pragma once

#include "SGX/SGSample.h"
#include <DirectXMath.h>

class Subresources : public ISGSample
{
public:
    Subresources(U32 width, U32 height, wchar_t const* windowName);

    void    OnInit() override;
    void    OnDestroy() override;

    void    OnUpdate() override;
    void    OnRender() override;

private:
    // Number of frame buffers
    static const uint32_t NumFrames = 3;

    SG_VIEWPORT m_Viewport;
    SG_RECT m_Scissor;

    ISGDevice* m_pDevice;
    ISGExecutionContext* m_pExecutionContext;
    ISGSwapChain* m_pSwapChain;
    ISGPipelineState* m_pPipelineState;

    ISGBuffer* m_pVertexBuffer;
    ISGBuffer* m_pIndexBuffer;

    ISGTexture* m_pDepthStencil;
    ISGDepthStencilView* m_pDSView;

    ISGSampler* m_pSampler;

    ISGTexture* m_pTexture;
    ISGShaderResourceView* m_pTextureSRV;

    ISGBuffer* m_pConstantBuffers[NumFrames];

    Camera m_Camera;
    TimeScaler m_TimeScaler;
    float m_CurrentAngle;
    uint32_t m_FrameIndex;

    static_assert(_countof(m_pConstantBuffers) == NumFrames, "Number of constant buffers must be equal NumFrames");

    void LoadPipelineState();
    void LoadAssets();

    void PrepareCommandList(ISGCommandList* pCommandList);
};
