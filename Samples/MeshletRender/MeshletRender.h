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

#pragma once

#include "SGX/SGSample.h"
#include <DirectXMath.h>
#include "Model.h"

class MeshletRender : public ISGSample
{
public:
    struct Constants
    {
        XMMATRIX World;
        XMMATRIX WorldView;
        XMMATRIX WorldViewProj;
        UINT     DrawMeshlets;
    };

    MeshletRender(U32 width, U32 height, wchar_t const* windowName);

    void OnInit() override;
    void OnDestroy() override;

    void OnUpdate() override;
    void OnRender() override;

private:
    // Number of frame buffers
    static const uint32_t NumFrames = 3;

    SG_VIEWPORT m_Viewport;
    SG_RECT m_Scissor;

    ISGDevice* m_pDevice;
    ISGExecutionContext* m_pExecutionContext;
    ISGSwapChain* m_pSwapChain;

    ISGPipelineState* m_pPipelineState;

    ISGBuffer* m_pConstantBuffers[NumFrames];

    ISGTexture* m_pDepthStencil;
    ISGDepthStencilView* m_pDSView;

    TimeScaler m_TimeScaler;
    Camera m_Camera;
    Model m_Model;
    U32 m_FrameIndex;
    float m_CurrentAngle;

    void LoadPipelineState();
    void LoadAssets();
    void PopulateCommandList(ISGCommandList* pCommandList);
};
