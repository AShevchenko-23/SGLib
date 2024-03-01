
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

#include <chrono>
#include <vector>
#include <DirectXMath.h>
#include <Include/SGLib.h>

using namespace DirectX;

typedef SgU8  U8;
typedef SgU16 U16;
typedef SgU32 U32;
typedef SgU64 U64;

#define DEG2RAD(deg) ((deg) / 180.0f * XM_PI)
#define SG_RELEASE(pObj) if (pObj != SG_NULL) { pObj->Release(); pObj = SG_NULL; }

constexpr U32 AlignValue(U32 value, U32 align)
{
    return (value + (align - 1)) / align * align;
}

namespace FastBufferDesc
{
    SG_BUFFER_DESC Constant(U32 sizeBytes);
    SG_BUFFER_DESC Upload(U32 sizeBytes);
    SG_BUFFER_DESC Structured(U32 sizeBytes, bool allowSRV, bool allowUAV, bool allowRAW);
    SG_BUFFER_DESC Vertex(U32 sizeBytes, bool allowSRV, bool allowUAV, bool allowRAW);
    SG_BUFFER_DESC Index(U32 sizeBytes, bool allowSRV, bool allowUAV, bool allowRAW);
}

namespace FastTextureDesc
{
    ///-------------------------------------------------------------------------------------------------
    /// Texture 1D
    ///-------------------------------------------------------------------------------------------------
    SG_TEXTURE_DESC Tex1D(SG_TEXTURE_TYPE type, U32 width, SG_FORMAT format, U32 mipLevels, bool allowSRV, bool allowUAV);
    SG_TEXTURE_DESC Tex1DArray(SG_TEXTURE_TYPE type, U32 width, SG_FORMAT format, U32 mipLevels, U32 arraySize, bool allowSRV, bool allowUAV);

    ///-------------------------------------------------------------------------------------------------
    /// Texture 2D
    ///-------------------------------------------------------------------------------------------------
    SG_TEXTURE_DESC Tex2D(SG_TEXTURE_TYPE type, U32 width, U32 height, SG_FORMAT format, U32 mipLevels, bool allowSRV, bool allowUAV);
    SG_TEXTURE_DESC Tex2DArray(SG_TEXTURE_TYPE type, U32 width, U32 height, SG_FORMAT format, U32 mipLevels, U32 arraySize, bool allowSRV, bool allowCubeSrv, bool allowUAV);

    ///-------------------------------------------------------------------------------------------------
    /// Texture 3D
    ///-------------------------------------------------------------------------------------------------
    SG_TEXTURE_DESC Tex3D(SG_TEXTURE_TYPE type, U32 width, U32 height, U32 depth, SG_FORMAT format, U32 mipLevels, bool allowSRV, bool allowUAV);
}

namespace FastViewDesc
{
    ///-------------------------------------------------------------------------------------------------
    /// ShaderResourceViews
    ///-------------------------------------------------------------------------------------------------
    SG_SHADER_RESOURCE_VIEW_DESC AsStructuredBuffer(U32 firstElement, U32 elementCount, U32 stride);
    SG_SHADER_RESOURCE_VIEW_DESC AsTypedBuffer(U32 firstElement, U32 elementCount, SG_FORMAT format);
    SG_SHADER_RESOURCE_VIEW_DESC AsByteaddressBuffer(U32 first32BitElement, U32 num32BitElements);

    SG_SHADER_RESOURCE_VIEW_DESC AsTexture(SG_FORMAT format, U32 mipSlice, U32 mipCount, U32 firstArraySlice, U32 planeSlice);
    SG_SHADER_RESOURCE_VIEW_DESC AsTextureArray(SG_FORMAT format, U32 mipSlice, U32 mipCount, U32 firstArraySlice, U32 arraySize, U32 planeSlice);
    SG_SHADER_RESOURCE_VIEW_DESC AsTextureCube(SG_FORMAT format, U32 mipSlice, U32 mipCount, U32 firstFace, U32 numCubes);

    ///-------------------------------------------------------------------------------------------------
    /// UnorderedAccessViews
    ///-------------------------------------------------------------------------------------------------
    SG_UNORDERED_ACCESS_VIEW_DESC AsRWStructuredBuffer(U32 firstElement, U32 elementCount, U32 stride);
    SG_UNORDERED_ACCESS_VIEW_DESC AsRWTypedBuffer(U32 firstElement, U32 elementCount, SG_FORMAT format);
    SG_UNORDERED_ACCESS_VIEW_DESC AsRWByteaddressBuffer(U32 first32BitElement, U32 num32BitElements);

    SG_UNORDERED_ACCESS_VIEW_DESC AsRWTexture(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 planeSlice);
    SG_UNORDERED_ACCESS_VIEW_DESC AsRWTextureArray(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 arraySize, U32 planeSlice);

    ///-------------------------------------------------------------------------------------------------
    /// RenderTargetViews
    ///-------------------------------------------------------------------------------------------------
    SG_RENDER_TARGET_VIEW_DESC AsRenderTarget(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 planeSlice);
    SG_RENDER_TARGET_VIEW_DESC AsRenderTarget(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 arraySize, U32 planeSlice);

    ///-------------------------------------------------------------------------------------------------
    /// DepthStencilViews
    ///-------------------------------------------------------------------------------------------------
    SG_DEPTH_STENCIL_VIEW_DESC AsDepthStencil(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice);
    SG_DEPTH_STENCIL_VIEW_DESC AsDepthStencil(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 arraySize);
}

/*
namespace FastSamplerDesc
{
    SG_SAMPLER_DESC Simple(SG_TEXTURE_ADDRESS_MODE addressMode, SG_FILTER_TYPE filterType, float maxLOD)
    {
        SG_SAMPLER_DESC samplerDesc{};
        samplerDesc.AddressU = addressMode;
        samplerDesc.AddressV = addressMode;
        samplerDesc.AddressW = addressMode;
        samplerDesc.Filter = SG_ENCODE_BASIC_FILTER(filterType, filterType, filterType, SG_FILTER_REDUCTION_STANDART);//SamplerFilter::CreateBasic(filterType, filterType, filterType);
        samplerDesc.MaxLOD = maxLOD;

        return samplerDesc;
    }
}
*/
typedef std::vector<uint8_t> ByteBuffer;
struct ImageDesc;

// Direct upload buffers (works only with SG_BUFFER_TYPE_UPLOAD)
void UploadBuffer(ISGBuffer* pBuffer, void const* pSrcData, U32 dataSize);

// Upload common buffers through CopyResource call
void UploadCommonBuffer(ISGDevice* pDevice, ISGCommandList* pCommandList, ISGBuffer* pDestBuffer, void const* pSrcData, U32 dataSize);

bool CreateUploadTextureFromData(ISGDevice* pDevice, ImageDesc const& imageDesc, ByteBuffer const& bitmap, ISGTexture** ppTexture);

// Load any binary file to the vector<char>
bool LoadBinaryFile(char const* pFilename, ByteBuffer& outBuffer);

struct ImageDesc
{
    uint32_t Width;
    uint32_t Height;
    SG_FORMAT Format;
};

bool LoadTGA(char const* pFileName, ImageDesc& outImageDesc, ByteBuffer& bitmap);

class TimeScaler
{
public:
    TimeScaler();

    void TimeStamp();

    float   ComputeScale() const { return m_LastDurationMS / m_TargetFrameTime; }
    float   GetDurationMS() const { return m_LastDurationMS; }
    U32     GetFrameRate() const { return m_FrameRate; }

private:
    typedef std::chrono::system_clock::time_point TimePoint;
    typedef std::chrono::microseconds Microseconds;

    TimePoint m_LastTimePoint;
    float m_TargetFrameTime;
    float m_LastDurationMS;

    float m_MsCounter;
    U32 m_FrameCounter;
    U32 m_FrameRate;
};

class Camera
{
public:
    Camera(bool isLeftHand);

    void SetPosition(DirectX::XMFLOAT3 const& position);
    void SetTarget(DirectX::XMFLOAT3 const& target);

    void SetFOV(float fov);
    void SetAspectRatio(float aspectRatio);
    void SetZRange(float nearZ, float farZ);

    void Update();

    XMMATRIX const& GetView() const { return m_View; }
    XMMATRIX const& GetProjection() const { return m_Projection; }
    XMMATRIX const& GetViewProjection() const { return m_ViewProjection; }

private:
    bool       m_IsLeftHand;
    XMFLOAT3   m_Position;
    XMFLOAT3   m_Target;

    float      m_NearZ;
    float      m_FarZ;
    float      m_FOV;
    float      m_AspectRatio;

    XMMATRIX m_View;
    XMMATRIX m_Projection;
    XMMATRIX m_ViewProjection;
};
