#pragma once
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

#include "SGX/SGSample.h"
#include <DirectXMath.h>

class QueriesSample : public ISGSample
{
public:
    QueriesSample(U32 width, U32 height, wchar_t const* windowName);

    void OnInit() override;
    void OnDestroy() override;

    void OnUpdate() override;
    void OnRender() override;

private:
    // Number of frame buffers
    static const uint32_t NumFrames = 3;

    struct SceneConstantBuffer
    {
        XMFLOAT4 offset;
    };

    SG_VIEWPORT m_Viewport;
    SG_RECT m_Scissor;

    ISGDevice* m_pDevice;
    ISGExecutionContext* m_pExecutionContext;
    ISGSwapChain* m_pSwapChain;

    ISGPipelineState* m_pPipelineState;
    ISGDepthStencilState* m_pDepthOnlyRead;

    ISGBlendState* m_pBlendStateRTOverride;
    ISGBlendState* m_pBlendStateNoBlendState;

    ISGBuffer* m_pVertexBuffer;

    ISGTexture* m_pDepthStencil;
    ISGDepthStencilView* m_pDSView;

    ISGBuffer* m_pCbFarQuad[NumFrames];
    ISGBuffer* m_pCbNearQuad[NumFrames];

    ISGPredicate* m_pPredicate;
    U32 m_FrameIndex;

    static_assert(_countof(m_pCbFarQuad) == NumFrames, "Number of constant buffers must be equal NumFrames");
    static_assert(_countof(m_pCbNearQuad) == NumFrames, "Number of constant buffers must be equal NumFrames");

    void LoadPipelineState();
    void LoadAssets();
    void PrepareCommandList(ISGCommandList* pCommandList);
};
