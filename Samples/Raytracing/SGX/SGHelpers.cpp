
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

#include "SGHelpers.h"
#include <stdint.h>
#include <fstream>


namespace FastBufferDesc
{
    SG_BUFFER_DESC Constant(U32 sizeBytes)
    {
        SG_BUFFER_DESC desc = {};
        desc.Type = SG_BUFFER_TYPE_UPLOAD;
        desc.BindFlags = SG_BUFFER_BIND_FLAG_CONSTANT_BUFFER;
        desc.Size = AlignValue(sizeBytes, SG_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
        return desc;
    }

    SG_BUFFER_DESC Upload(U32 sizeBytes)
    {
        SG_BUFFER_DESC desc = {};
        desc.Type = SG_BUFFER_TYPE_UPLOAD;
        desc.BindFlags = SG_BUFFER_BIND_FLAG_NONE;
        desc.Size = sizeBytes;
        return desc;
    }

    SG_BUFFER_DESC Structured(U32 sizeBytes, bool allowSRV, bool allowUAV, bool allowRAW)
    {
        SG_BUFFER_DESC desc = {};
        desc.Type = SG_BUFFER_TYPE_COMMON;
        desc.BindFlags |= allowSRV ? SG_BUFFER_BIND_FLAG_SHADER_RESOURCE : SG_BUFFER_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_BUFFER_BIND_FLAG_UNORDERED_ACCESS : SG_BUFFER_BIND_FLAG_NONE;
        desc.BindFlags |= allowRAW ? SG_BUFFER_BIND_FLAG_BYTE_ADDRESS : SG_BUFFER_BIND_FLAG_NONE;
        desc.Size = sizeBytes;
        return desc;
    }

    SG_BUFFER_DESC Vertex(U32 sizeBytes, bool allowSRV, bool allowUAV, bool allowRAW)
    {
        SG_BUFFER_DESC desc = {};
        desc.Type = SG_BUFFER_TYPE_COMMON;
        desc.BindFlags |= SG_BUFFER_BIND_FLAG_VERTEX_BUFFER;
        desc.BindFlags |= allowSRV ? SG_BUFFER_BIND_FLAG_SHADER_RESOURCE : SG_BUFFER_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_BUFFER_BIND_FLAG_UNORDERED_ACCESS : SG_BUFFER_BIND_FLAG_NONE;
        desc.BindFlags |= allowRAW ? SG_BUFFER_BIND_FLAG_BYTE_ADDRESS : SG_BUFFER_BIND_FLAG_NONE;
        desc.Size = sizeBytes;
        return desc;
    }

    SG_BUFFER_DESC Index(U32 sizeBytes, bool allowSRV, bool allowUAV, bool allowRAW)
    {
        SG_BUFFER_DESC desc = {};
        desc.Type = SG_BUFFER_TYPE_COMMON;
        desc.BindFlags |= SG_BUFFER_BIND_FLAG_INDEX_BUFFER;
        desc.BindFlags |= allowSRV ? SG_BUFFER_BIND_FLAG_SHADER_RESOURCE : SG_BUFFER_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_BUFFER_BIND_FLAG_UNORDERED_ACCESS : SG_BUFFER_BIND_FLAG_NONE;
        desc.BindFlags |= allowRAW ? SG_BUFFER_BIND_FLAG_BYTE_ADDRESS : SG_BUFFER_BIND_FLAG_NONE;
        desc.Size = sizeBytes;
        return desc;
    }
}

namespace FastTextureDesc
{
    ///-------------------------------------------------------------------------------------------------
    /// Texture 1D
    ///-------------------------------------------------------------------------------------------------
    SG_TEXTURE_DESC Tex1D(SG_TEXTURE_TYPE type, U32 width, SG_FORMAT format, U32 mipLevels, bool allowSRV, bool allowUAV)
    {
        SG_TEXTURE_DESC desc = {};
        desc.Type = type;
        desc.BindFlags |= allowSRV ? SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE : SG_TEXTURE_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_TEXTURE_BIND_FLAG_UNORDERED_ACCESS : SG_TEXTURE_BIND_FLAG_NONE;

        desc.Dimension = SG_TEXTURE_DIMENSION_1D;
        desc.Format = format;
        desc.Width = width;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = mipLevels;
        return desc;
    }

    SG_TEXTURE_DESC Tex1DArray(SG_TEXTURE_TYPE type, U32 width, SG_FORMAT format, U32 mipLevels, U32 arraySize, bool allowSRV, bool allowUAV)
    {
        SG_TEXTURE_DESC desc = {};
        desc.Type = type;
        desc.BindFlags |= allowSRV ? SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE : SG_TEXTURE_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_TEXTURE_BIND_FLAG_UNORDERED_ACCESS : SG_TEXTURE_BIND_FLAG_NONE;

        desc.Dimension = SG_TEXTURE_DIMENSION_1D;
        desc.Format = format;
        desc.Width = width;
        desc.Height = 1;
        desc.DepthOrArraySize = arraySize;
        desc.MipLevels = mipLevels;
        return desc;
    }

    ///-------------------------------------------------------------------------------------------------
    /// Texture 2D
    ///-------------------------------------------------------------------------------------------------
    SG_TEXTURE_DESC Tex2D(SG_TEXTURE_TYPE type, U32 width, U32 height, SG_FORMAT format, U32 mipLevels, bool allowSRV, bool allowUAV)
    {
        SG_TEXTURE_DESC desc = {};
        desc.Type = type;
        desc.BindFlags |= allowSRV ? SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE : SG_TEXTURE_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_TEXTURE_BIND_FLAG_UNORDERED_ACCESS : SG_TEXTURE_BIND_FLAG_NONE;

        desc.Dimension = SG_TEXTURE_DIMENSION_2D;
        desc.Format = format;
        desc.Width = width;
        desc.Height = height;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = mipLevels;

        return desc;
    }

    SG_TEXTURE_DESC Tex2DArray(SG_TEXTURE_TYPE type, U32 width, U32 height, SG_FORMAT format, U32 mipLevels, U32 arraySize, bool allowSRV, bool allowCubeSrv, bool allowUAV)
    {
        SG_TEXTURE_DESC desc = {};
        desc.Type = type;
        desc.BindFlags |= allowSRV ? SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE : SG_TEXTURE_BIND_FLAG_NONE;
        desc.BindFlags |= allowCubeSrv ? SG_TEXTURE_BIND_FLAG_TEXTURE_CUBE : SG_TEXTURE_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_TEXTURE_BIND_FLAG_UNORDERED_ACCESS : SG_TEXTURE_BIND_FLAG_NONE;

        desc.Dimension = SG_TEXTURE_DIMENSION_2D;
        desc.Format = format;
        desc.Width = width;
        desc.Height = height;
        desc.DepthOrArraySize = arraySize;
        desc.MipLevels = mipLevels;

        return desc;
    }

    ///-------------------------------------------------------------------------------------------------
    /// Texture 3D
    ///-------------------------------------------------------------------------------------------------
    SG_TEXTURE_DESC Tex3D(SG_TEXTURE_TYPE type, U32 width, U32 height, U32 depth, SG_FORMAT format, U32 mipLevels, bool allowSRV, bool allowUAV)
    {
        SG_TEXTURE_DESC desc = {};
        desc.Type = type;
        desc.BindFlags |= allowSRV ? SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE : SG_TEXTURE_BIND_FLAG_NONE;
        desc.BindFlags |= allowUAV ? SG_TEXTURE_BIND_FLAG_UNORDERED_ACCESS : SG_TEXTURE_BIND_FLAG_NONE;

        desc.Dimension = SG_TEXTURE_DIMENSION_2D;
        desc.Format = format;
        desc.Width = width;
        desc.Height = height;
        desc.DepthOrArraySize = depth;
        desc.MipLevels = mipLevels;

        return desc;
    }
}

namespace FastViewDesc
{
    ///-------------------------------------------------------------------------------------------------
    /// ShaderResourceViews
    ///-------------------------------------------------------------------------------------------------
    SG_SHADER_RESOURCE_VIEW_DESC AsStructuredBuffer(U32 firstElement, U32 elementCount, U32 stride)
    {
        SG_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Type = SG_SRV_TYPE_STRUCTURED_BUFFER;
        srvDesc.StructuredBuffer.FirstElement   = firstElement;
        srvDesc.StructuredBuffer.ElementCount   = elementCount;
        srvDesc.StructuredBuffer.Stride         = stride;

        return srvDesc;
    }

    SG_SHADER_RESOURCE_VIEW_DESC AsTypedBuffer(U32 firstElement, U32 elementCount, SG_FORMAT format)
    {
        SG_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Type = SG_SRV_TYPE_TYPED_BUFFER;
        srvDesc.TypedBuffer.FirstElement   = firstElement;
        srvDesc.TypedBuffer.ElementCount   = elementCount;
        srvDesc.TypedBuffer.Format         = format;

        return srvDesc;
    }

    SG_SHADER_RESOURCE_VIEW_DESC AsByteaddressBuffer(U32 first32BitElement, U32 num32BitElements)
    {
        SG_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Type = SG_SRV_TYPE_BYTE_ADDRESS_BUFFER;
        srvDesc.ByteAddressBuffer.First32BitElement = first32BitElement;
        srvDesc.ByteAddressBuffer.Num32BitElements  = num32BitElements;

        return srvDesc;
    }

    SG_SHADER_RESOURCE_VIEW_DESC AsTexture(SG_FORMAT format, U32 mipSlice, U32 mipCount, U32 firstArraySlice, U32 planeSlice)
    {
        SG_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Type = SG_SRV_TYPE_TEXTURE;
        srvDesc.Texture.Format          = format;
        srvDesc.Texture.MostDetailedMip = mipSlice;
        srvDesc.Texture.MipLevels       = mipCount;
        srvDesc.Texture.FirstArraySlice = firstArraySlice;
        srvDesc.Texture.ArraySize       = 1;
        srvDesc.Texture.PlaneSlice      = planeSlice;

        return srvDesc;
    }

    SG_SHADER_RESOURCE_VIEW_DESC AsTextureArray(SG_FORMAT format, U32 mipSlice, U32 mipCount, U32 firstArraySlice, U32 arraySize, U32 planeSlice)
    {
        SG_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Type = SG_SRV_TYPE_TEXTURE;
        srvDesc.Texture.Format          = format;
        srvDesc.Texture.MostDetailedMip = mipSlice;
        srvDesc.Texture.MipLevels       = mipCount;
        srvDesc.Texture.FirstArraySlice = firstArraySlice;
        srvDesc.Texture.ArraySize       = arraySize;
        srvDesc.Texture.PlaneSlice      = planeSlice;

        return srvDesc;
    }

    SG_SHADER_RESOURCE_VIEW_DESC AsTextureCube(SG_FORMAT format, U32 mipSlice, U32 mipCount, U32 firstFace, U32 numCubes)
    {
        SG_SHADER_RESOURCE_VIEW_DESC srvDesc{};
        srvDesc.Type = SG_SRV_TYPE_TEXTURE_CUBE;
        srvDesc.TextureCube.Format          = format;
        srvDesc.TextureCube.MostDetailedMip = mipSlice;
        srvDesc.TextureCube.MipLevels       = mipCount;
        srvDesc.TextureCube.FirstFace       = firstFace;
        srvDesc.TextureCube.NumCubes        = numCubes;

        return srvDesc;
    }

    ///-------------------------------------------------------------------------------------------------
    /// UnorderedAccessViews
    ///-------------------------------------------------------------------------------------------------
    SG_UNORDERED_ACCESS_VIEW_DESC AsRWStructuredBuffer(U32 firstElement, U32 elementCount, U32 stride)
    {
        SG_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.Type = SG_UAV_TYPE_STRUCTURED_BUFFER;
        uavDesc.StructuredBuffer.FirstElement   = firstElement;
        uavDesc.StructuredBuffer.ElementCount   = elementCount;
        uavDesc.StructuredBuffer.Stride         = stride;

        return uavDesc;
    }

    SG_UNORDERED_ACCESS_VIEW_DESC AsRWTypedBuffer(U32 firstElement, U32 elementCount, SG_FORMAT format)
    {
        SG_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.Type = SG_UAV_TYPE_TYPED_BUFFER;
        uavDesc.TypedBuffer.FirstElement   = firstElement;
        uavDesc.TypedBuffer.ElementCount   = elementCount;
        uavDesc.TypedBuffer.Format         = format;

        return uavDesc;
    }

    SG_UNORDERED_ACCESS_VIEW_DESC AsRWByteaddressBuffer(U32 first32BitElement, U32 num32BitElements)
    {
        SG_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.Type = SG_UAV_TYPE_BYTE_ADDRESS_BUFFER;
        uavDesc.ByteAddressBuffer.First32BitElement   = first32BitElement;
        uavDesc.ByteAddressBuffer.Num32BitElements    = num32BitElements;

        return uavDesc;
    }

    SG_UNORDERED_ACCESS_VIEW_DESC AsRWTexture(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 planeSlice)
    {
        SG_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.Type = SG_UAV_TYPE_TEXTURE;
        uavDesc.Texture.Format          = format;
        uavDesc.Texture.MipSlice        = mipSlice;
        uavDesc.Texture.FirstArraySlice = firstArraySlice;
        uavDesc.Texture.ArraySize       = 1;
        uavDesc.Texture.PlaneSlice      = planeSlice;

        return uavDesc;
    }

    SG_UNORDERED_ACCESS_VIEW_DESC AsRWTextureArray(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 arraySize, U32 planeSlice)
    {
        SG_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
        uavDesc.Type = SG_UAV_TYPE_TEXTURE;
        uavDesc.Texture.Format          = format;
        uavDesc.Texture.MipSlice        = mipSlice;
        uavDesc.Texture.FirstArraySlice = firstArraySlice;
        uavDesc.Texture.ArraySize       = arraySize;
        uavDesc.Texture.PlaneSlice      = planeSlice;

        return uavDesc;
    }

    ///-------------------------------------------------------------------------------------------------
    /// RenderTargetViews
    ///-------------------------------------------------------------------------------------------------
    SG_RENDER_TARGET_VIEW_DESC AsRenderTarget(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 planeSlice)
    {
        SG_RENDER_TARGET_VIEW_DESC rtvDesc{};
        rtvDesc.Format          = format;
        rtvDesc.MipSlice        = mipSlice;
        rtvDesc.FirstArraySlice = firstArraySlice;
        rtvDesc.ArraySize       = 1;
        rtvDesc.PlaneSlice      = planeSlice;

        return rtvDesc;
    }

    SG_RENDER_TARGET_VIEW_DESC AsRenderTarget(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 arraySize, U32 planeSlice)
    {
        SG_RENDER_TARGET_VIEW_DESC rtvDesc{};
        rtvDesc.Format          = format;
        rtvDesc.MipSlice        = mipSlice;
        rtvDesc.FirstArraySlice = firstArraySlice;
        rtvDesc.ArraySize       = arraySize;
        rtvDesc.PlaneSlice      = planeSlice;

        return rtvDesc;
    }

    ///-------------------------------------------------------------------------------------------------
    /// DepthStencilViews
    ///-------------------------------------------------------------------------------------------------
    SG_DEPTH_STENCIL_VIEW_DESC AsDepthStencil(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice)
    {
        SG_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
        dsvDesc.Format          = format;
        dsvDesc.MipSlice        = mipSlice;
        dsvDesc.FirstArraySlice = firstArraySlice;
        dsvDesc.ArraySize       = 1;

        return dsvDesc;
    }

    SG_DEPTH_STENCIL_VIEW_DESC AsDepthStencil(SG_FORMAT format, U32 mipSlice, U32 firstArraySlice, U32 arraySize)
    {
        SG_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
        dsvDesc.Format          = format;
        dsvDesc.MipSlice        = mipSlice;
        dsvDesc.FirstArraySlice = firstArraySlice;
        dsvDesc.ArraySize       = arraySize;

        return dsvDesc;
    }
}

void UploadBuffer(ISGBuffer* pBuffer, void const* pSrcData, U32 dataSize)
{
    assert(pBuffer != nullptr);

    SG_BUFFER_DESC desc;
    pBuffer->GetDesc(&desc);

    assert(desc.Type == SG_BUFFER_TYPE_UPLOAD);

    void* pDest = nullptr;
    if (pBuffer->Map(&pDest) == SG_OK)
    {
        memcpy(pDest, pSrcData, dataSize);
        pBuffer->Unmap();
    }
}

void UploadCommonBuffer(ISGDevice* pDevice, ISGCommandList* pCommandList, ISGBuffer* pDestBuffer, void const* pSrcData, U32 dataSize)
{
    assert(pDestBuffer != nullptr);

    SG_BUFFER_DESC desc;
    pDestBuffer->GetDesc(&desc);

    // Destination buffer should be common.
    assert(desc.Type == SG_BUFFER_TYPE_COMMON);

    // But an immediate one we make as upload with no any binding flags.
    desc.Type = SG_BUFFER_TYPE_UPLOAD;
    desc.BindFlags = SG_BUFFER_BIND_FLAG_NONE;

    ISGBuffer* pUploadBuffer = nullptr;
    if (pDevice->CreateBuffer(&desc, &pUploadBuffer) != SG_OK)
        throw std::exception("Failed to create upload buffer");

    void* pDest = nullptr;
    if (pUploadBuffer->Map(&pDest) != SG_OK)
        throw std::exception("Failed to map upload buffer");

    memcpy(pDest, pSrcData, dataSize);
    pUploadBuffer->Unmap();

    pCommandList->CopyResource(pDestBuffer, pUploadBuffer);
    pUploadBuffer->Release();
}

void UploadCommonTexture(ISGDevice* pDevice, ISGCommandList* pCommandList, ISGTexture* pDestTexture, ImageDesc const& sourceImage, ByteBuffer const& bitmap)
{
    SG_TEXTURE_DESC uploadDesc{};
    pDestTexture->GetDesc(&uploadDesc);

    uploadDesc.Type = SG_TEXTURE_TYPE_UPLOAD;
    uploadDesc.BindFlags = SG_TEXTURE_BIND_FLAG_NONE;

    ISGTexture* pUploadTexture = nullptr;
    pDevice->CreateTexture(&uploadDesc, &pUploadTexture);

    U32 formatSize = SgGetFormatSize(uploadDesc.Format);
    U64 sourceRowSize = formatSize * static_cast<U64>(sourceImage.Width);

    ISGSubresource* pSubresource = nullptr;
    pUploadTexture->GetSubresource(0, 0, 0, &pSubresource);

    SG_MAPPED_SUBRESOURCE mappedSubresource;
    if (pSubresource->Map(&mappedSubresource) == SG_OK)
    {
        uint8_t* pDest = (uint8_t*)mappedSubresource.pData;
        uint8_t const* pSource = bitmap.data();

        // Copying data to the upload texture according to got parameters
        for (U32 row = 0; row < sourceImage.Height; row++)
        {
            memcpy(pDest, pSource, sourceRowSize);

            pDest += mappedSubresource.RowPitch;
            pSource += sourceRowSize;
        }

        pSubresource->Unmap();
    }
    pSubresource->Release();

    pCommandList->CopyResource(pDestTexture, pUploadTexture);
    pUploadTexture->Release();
}

bool CreateUploadTextureFromData(ISGDevice* pDevice, ImageDesc const& imageDesc, ByteBuffer const& bitmap, ISGTexture** ppTexture)
{
    SG_TEXTURE_DESC desc = FastTextureDesc::Tex2D(SG_TEXTURE_TYPE_UPLOAD, imageDesc.Width, imageDesc.Height, imageDesc.Format, 1, false, false);

    ISGTexture* pTexture;

    if (pDevice->CreateTexture(&desc, &pTexture) != SG_OK)
        return false;

    ISGSubresource* pSubresource = nullptr;
    if (pTexture->GetSubresource(0, 0, 0, &pSubresource) == SG_OK)
    {
        SG_MAPPED_SUBRESOURCE mappedSubresource;
        if (pSubresource->Map(&mappedSubresource) == SG_OK)
        {
            uint32_t const formatSize = SgGetFormatSize(desc.Format);
            uint64_t const sourceRowSize = formatSize * static_cast<uint64_t>(imageDesc.Width);

            uint8_t* pDest = (uint8_t*)mappedSubresource.pData;
            uint8_t const* pSource = bitmap.data();

            for (U32 row = 0; row < imageDesc.Height; row++)
            {
                memcpy(pDest, pSource, sourceRowSize);

                pDest += mappedSubresource.RowPitch;
                pSource += sourceRowSize;
            }

            pSubresource->Unmap();
        }
        pSubresource->Release();
    }

    *ppTexture = pTexture;
    return true;
}

bool LoadBinaryFile(char const* pFilename, ByteBuffer& outBuffer)
{
    std::ifstream file(pFilename, std::ios::binary | std::ios::ate);

    if (!file.is_open())
        return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    outBuffer.resize(size);

    if (file.read((char*)outBuffer.data(), size))
    {
        return true;
    }

    return false;
}

bool LoadTGA(char const* pFilename, ImageDesc& outImageDesc, ByteBuffer& bitmap)
{
    #pragma pack(1)
    struct ColorMapDesc
    {
        uint16_t FirstItem;
        uint16_t NumItems;
        uint8_t  BitsPerItem;
    };

    struct ImageDescription
    {
        uint16_t OffsetX;
        uint16_t OffsetY;

        uint16_t Width;
        uint16_t Height;

        uint8_t  BPP;
        uint8_t  Descriptor;
    };

    struct TargaHeader
    {
        uint8_t IdentifierLength;
        uint8_t ColorMapType;
        uint8_t ImageType;
        ColorMapDesc ColorMapDesc;
        ImageDescription ImageDesc;
    };
    #pragma pack()

    TargaHeader header{};

    std::ifstream file(pFilename, std::ios::binary | std::ios::ate);

    if (!file.is_open())
        return false;

    file.seekg(0, std::ios::beg);

    if (!file.read((char*)&header, sizeof(TargaHeader)))
        return false;

    ImageDescription const& imageDesc = header.ImageDesc;

    bool IsTrueColor = (header.ImageType & 0x3) == 0x2;
    bool IsCompressed = header.ImageType & 0x8;
    bool Is24or32bpp = (imageDesc.BPP == 24) || (imageDesc.BPP == 32);

    bool hFlipped = imageDesc.Descriptor & 0x10;
    bool vFlipped = (imageDesc.Descriptor & 0x20) == 0;

    if (!IsTrueColor || IsCompressed || !Is24or32bpp)
        return false;

    // Seek ID, if it is
    if (!file.seekg(header.IdentifierLength, std::ios_base::cur))
        return false;

    outImageDesc.Width = static_cast<uint32_t>(imageDesc.Width);
    outImageDesc.Height = static_cast<uint32_t>(imageDesc.Height);
    outImageDesc.Format = SG_FORMAT_B8G8R8A8_UNORM;

    uint8_t tgaBPP = imageDesc.BPP / 8; // Bit to bytes
    uint8_t outBPP = SgGetFormatSize(outImageDesc.Format);

    uint32_t totalPixels = imageDesc.Width * imageDesc.Height;
    uint32_t tgaDataSize = tgaBPP * totalPixels;

    bitmap.resize( outBPP * totalPixels );

    if (!file.read((char*)bitmap.data(), tgaDataSize))
    {
        outImageDesc = {};
        return false;
    }

    #pragma pack(1)
    struct _24bit
    {
        uint8_t B, G, R;
    };

    struct _32bit
    {
        uint8_t B, G, R, A;
    };
    #pragma pack()

    static_assert(sizeof(_24bit) == 3, "");
    static_assert(sizeof(_32bit) == 4, "");

    if (tgaBPP == 3 && outBPP == 4)
    {
        // Stretch the image data from 24 bpp to 32 bpp

        _32bit* pDestMap = reinterpret_cast<_32bit*>(bitmap.data());
        _24bit* pSrcMap = reinterpret_cast<_24bit*>(bitmap.data());

        for (int i = totalPixels - 1; i >= 0; i--)
        {
            _24bit srcPixelData = pSrcMap[i];

            _32bit dstPixelData;
            dstPixelData.B = srcPixelData.B;
            dstPixelData.G = srcPixelData.G;
            dstPixelData.R = srcPixelData.R;
            dstPixelData.A = 0xFF;
            pDestMap[i] = dstPixelData;
        }
    }

    // Flip the image
    if (vFlipped)
    {
        uint32_t rowSize = imageDesc.Width * outBPP;
        void* pTmpRow = malloc(rowSize);

        for (uint16_t i = 0; i < imageDesc.Height / 2; i++)
        {
            uint32_t lastIndex = imageDesc.Height - i - 1;

            uint8_t* pFirstRow = (uint8_t*)bitmap.data() + rowSize * i;
            uint8_t* pLastRow = (uint8_t*)bitmap.data() + rowSize * lastIndex;
            memcpy(pTmpRow, pFirstRow, rowSize);
            memcpy(pFirstRow, pLastRow, rowSize);
            memcpy(pLastRow, pTmpRow, rowSize);
        }

        delete pTmpRow;
    }

    if (hFlipped)
    {
        // TODO: implement
    }

    return true;
}

TimeScaler::TimeScaler()
    : m_LastTimePoint(std::chrono::system_clock::now())
    , m_TargetFrameTime(1000.0f / 60.0f)
    , m_LastDurationMS(0.0f)
    , m_MsCounter(0.0f)
    , m_FrameCounter(0)
    , m_FrameRate(0)
{
}

void TimeScaler::TimeStamp()
{
    TimePoint timePoint = std::chrono::system_clock::now();
    Microseconds duration = std::chrono::duration_cast<Microseconds>(timePoint - m_LastTimePoint);
    m_LastDurationMS = duration.count() / 1000.0f;

    m_MsCounter += m_LastDurationMS;
    m_FrameCounter++;

    if (m_MsCounter >= 1000.0f)
    {
        m_FrameRate = m_FrameCounter / m_MsCounter * 1000.0f;
        m_FrameCounter = 0;
        m_MsCounter = 0;
    }

    m_LastTimePoint = timePoint;
}

    Camera::Camera(bool isLeftHand)
        : m_IsLeftHand(isLeftHand)
        , m_Position(0.0f, 0.0f, 0.0f)
        , m_Target(0.0f, 0.0f, 0.0f)
        , m_NearZ(0.0f)
        , m_FarZ(1.0f)
        , m_FOV(DEG2RAD(45))
        , m_AspectRatio(1.0f)
        , m_ViewProjection(XMMatrixIdentity())
    {
    }

    void Camera::SetPosition(XMFLOAT3 const& position)
    {
        m_Position = position;
    }

    void Camera::SetTarget(XMFLOAT3 const& target)
    {
        m_Target = target;
    }

    void Camera::SetFOV(float fov)
    {
        m_FOV = fov;
    }

    void Camera::SetAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
    }

    void Camera::SetZRange(float nearZ, float farZ)
    {
        m_NearZ = nearZ;
        m_FarZ = farZ;
    }

    void Camera::Update()
    {
        XMVECTOR position = { m_Position.x, m_Position.y, m_Position.z};
        XMVECTOR target = { m_Target.x, m_Target.y, m_Target.z};
        XMVECTOR upVector = { 0.0f, 1.0f, 0.0f};

        if (m_IsLeftHand)
        {
            m_Projection = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);
            m_View = XMMatrixLookAtLH(position, target, upVector);
        }
        else
        {
            m_Projection = XMMatrixPerspectiveFovRH(m_FOV, m_AspectRatio, m_NearZ, m_FarZ);
            m_View = XMMatrixLookAtRH(position, target, upVector);
        }

        m_ViewProjection = XMMatrixMultiply(m_View, m_Projection);
    }
