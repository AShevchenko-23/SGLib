
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

class RaytracingSample : public ISGSample
{
public:
    RaytracingSample(U32 width, U32 height, wchar_t const* windowName);

    void OnInit() override;
    void OnDestroy() override;

    void OnUpdate() override;
    void OnRender() override;

private:
    struct Matrix3x4
    {
        float Values[3][4];
    };

    struct Box
    {
        XMFLOAT3            BasePosition;
        U8                  HitGroupIndex;
        bool                AlphaTest;

        ISGBottomLevelAS*   pBLAS;
    };

    static const U32 NumFrames = 3;
    static const U32 NumBoxes = 3;

    ISGDevice* m_pDevice;
    ISGExecutionContext* m_pExecutionContext;
    ISGSwapChain* m_pSwapChain;

    ISGPipelineState* m_pPipelineState;

    ISGBuffer* m_pVertexBuffer;
    ISGShaderResourceView* m_pVertexBufferSRV;

    ISGBuffer* m_pIndexBuffer;
    ISGShaderResourceView* m_pIndexBufferSRV;

    ISGBuffer* m_pConstantBuffer;
    ISGBuffer* m_pTransformBuffers[NumFrames];
    Matrix3x4 m_TransformData[NumBoxes];

    ISGTexture* m_pOutTexture;
    ISGUnorderedAccessView* m_pOutTextureUAV;

    ISGTexture* m_pSimpleTexture;
    ISGShaderResourceView* m_pSimpleTextureSRV;
    ISGTexture* m_pAlphaTestTexture;
    ISGShaderResourceView* m_pAlphaTestTextureSRV;

    ISGSampler* m_pSampler;

    ISGBottomLevelAS* m_pFloorBLAS;
    ISGTopLevelAS* m_pSceneBVH;

    Box m_Boxes[NumBoxes];
    U32 m_FrameIndex;
    float m_Time;
    TimeScaler m_TimeScaler;

    static_assert(_countof(m_pTransformBuffers) == NumFrames, "Number of transform buffers must be equal NumFrames");

    void LoadPipelineState();
    void LoadAssets();
    void PrepareCommandList(ISGCommandList* pCommandList);

    void CalculateGeometryTransform(float time, Matrix3x4* pGeometryTransform);
    void CalculateInstanceTransform(float time, XMFLOAT3 const& basePosition, Matrix3x4* pInstanceTransform);
};
