
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

#include "CoreTypes.h"
#include "Format.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGResource);
SG_DECLARE_CLASS(ISGBuffer);
SG_DECLARE_CLASS(ISGSubresource);
SG_DECLARE_CLASS(ISGTexture);

#define SG_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT ( 256 )

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_RESOURCE_TYPE
{
    SG_RESOURCE_TYPE_UNDEFINED = 0,
    SG_RESOURCE_TYPE_BUFFER = 1,
    SG_RESOURCE_TYPE_TEXTURE = 2,
    SG_RESOURCE_TYPE_ACCELERATION_STRUCTURE = 3,
} SG_RESOURCE_TYPE;

typedef enum SG_BUFFER_TYPE
{
    SG_BUFFER_TYPE_COMMON = 0,
    SG_BUFFER_TYPE_UPLOAD = 1,
    SG_BUFFER_TYPE_READBACK = 2,
} SG_BUFFER_TYPE;

typedef enum SG_BUFFER_BIND_FLAGS
{
    SG_BUFFER_BIND_FLAG_NONE = 0,
    SG_BUFFER_BIND_FLAG_CONSTANT_BUFFER = 0x1,
    SG_BUFFER_BIND_FLAG_VERTEX_BUFFER = 0x2,
    SG_BUFFER_BIND_FLAG_INDEX_BUFFER = 0x4,

    SG_BUFFER_BIND_FLAG_SHADER_RESOURCE = 0x8,
    SG_BUFFER_BIND_FLAG_UNORDERED_ACCESS = 0x10,
    SG_BUFFER_BIND_FLAG_BYTE_ADDRESS = 0x20,
} SG_BUFFER_BIND_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_BUFFER_BIND_FLAGS);

typedef enum SG_TEXTURE_TYPE
{
    SG_TEXTURE_TYPE_COMMON = 0,
    SG_TEXTURE_TYPE_DEPTH_STENCIL = 1,
    SG_TEXTURE_TYPE_UPLOAD = 2,
    SG_TEXTURE_TYPE_READBACK = 3
} SG_TEXTURE_TYPE;

typedef enum SG_TEXTURE_DIMENSION
{
    SG_TEXTURE_DIMENSION_UNDEFINED = 0,
    SG_TEXTURE_DIMENSION_1D = 1,
    SG_TEXTURE_DIMENSION_2D = 2,
    SG_TEXTURE_DIMENSION_3D = 3,
} SG_TEXTURE_DIMENSION;

typedef enum SG_TEXTURE_BIND_FLAGS
{
    SG_TEXTURE_BIND_FLAG_NONE = 0,
    SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE = 0x1,
    SG_TEXTURE_BIND_FLAG_UNORDERED_ACCESS = 0x2,
    SG_TEXTURE_BIND_FLAG_RENDER_TARGET = 0x4,
    SG_TEXTURE_BIND_FLAG_TEXTURE_CUBE = 0x8,
} SG_TEXTURE_BIND_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_TEXTURE_BIND_FLAGS);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SG_BUFFER_DESC
{
    SG_BUFFER_TYPE          Type;
    SG_BUFFER_BIND_FLAGS    BindFlags;
    SgU32                   Size;
} SG_BUFFER_DESC;

typedef struct SG_CLEAR_VALUE
{
    SG_FORMAT Format;
    union
    {
        SG_COLOR_4F Color;
        struct
        {
            float Depth;
            SgU8 Stencil;
        };
    };
} SG_CLEAR_VALUE;

typedef struct SG_TEXTURE_DESC
{
    SG_TEXTURE_TYPE         Type;
    SG_TEXTURE_BIND_FLAGS   BindFlags;
    SG_TEXTURE_DIMENSION    Dimension;
    SG_FORMAT               Format;
    SgU32                   Width;
    SgU32                   Height;
    SgU32                   DepthOrArraySize;
    SgU32                   MipLevels;

    SgU8                    SampleCount;
    SgU8                    SampleQuality;

    SG_CLEAR_VALUE          DefaultValue;
} SG_TEXTURE_DESC;

typedef struct SG_SUBRESOURCE_INFO
{
    SgU32 MipLevels;
    SgU32 ArraySize;
    SgU32 PlaneSlices;
} SG_SUBRESOURCE_INFO;

typedef struct SG_MAPPED_SUBRESOURCE
{
    void* pData;
    SgU64 RowPitch;
    SgU64 DepthPitch;
} SG_MAPPED_SUBRESOURCE;


typedef struct SG_TEXTURE1D_COORDS
{
    SgU32 Offset;
    SgU32 ArraySlice;
    SgU32 Mip;
} SG_TEXTURE1D_COORDS;

typedef struct SG_TEXTURE2D_COORDS
{
    SgU32 Left;
    SgU32 Top;
    SgU32 ArraySlice;
    SgU32 Mip;
} SG_TEXTURE2D_COORDS;

typedef struct SG_TEXTURE3D_COORDS
{
    SgU32 Left;
    SgU32 Top;
    SgU32 Front;
    SgU32 Mip;
} SG_TEXTURE3D_COORDS;

typedef struct SG_TEXTURE_COPY_DESTINATION
{
    SG_TEXTURE_DIMENSION Dimension;
    union
    {
        SG_TEXTURE1D_COORDS Tex1D;
        SG_TEXTURE2D_COORDS Tex2D;
        SG_TEXTURE3D_COORDS Tex3D;
    };
} SG_TEXTURE_COPY_DESTINATION;

typedef struct SG_TEXTURE1D_REGION
{
    SgU32 Offset;
    SgU32 Width;
    SgU32 ArraySlice;
    SgU32 Mip;
} SG_TEXTURE1D_REGION;

typedef struct SG_TEXTURE2D_REGION
{
    SgU32 Left;
    SgU32 Top;
    SgU32 Width;
    SgU32 Height;
    SgU32 ArraySlice;
    SgU32 Mip;
} SG_TEXTURE2D_REGION;

typedef struct SG_TEXTURE3D_REGION
{
    SgU32 Left;
    SgU32 Top;
    SgU32 Width;
    SgU32 Height;
    SgU32 Front;
    SgU32 Back;
    SgU32 Mip;
} SG_TEXTURE3D_REGION;

typedef struct SG_TEXTURE_COPY_SOURCE
{
    SG_TEXTURE_DIMENSION Dimension;
    union
    {
        SG_TEXTURE1D_REGION Tex1D;
        SG_TEXTURE2D_REGION Tex2D;
        SG_TEXTURE3D_REGION Tex3D;
    };
} SG_TEXTURE_COPY_SOURCE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE SG_API ISGResource : public ISGObject
{
public:
    virtual SG_RESOURCE_TYPE SG_CALL GetType() = 0;

    virtual void SG_CALL SetName(
        char const* pName) = 0;

    virtual void SG_CALL GetName(
        SgU32* pStrLen,
        char* pName) = 0;
};

#else
typedef struct ISGResourceVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGResource* pThis);

    SgU32 (SG_CALL *Release)(
        ISGResource* pThis);

    // ISGResource
    SG_RESOURCE_TYPE (SG_CALL *GetType)(
        ISGResource* pThis);

    void (SG_CALL *SetName)(
        ISGResource* pThis, char const* pName);

    void (SG_CALL *GetName)(
        ISGResource* pThis,
        SgU32* pStrLen,
        char* pName);

} ISGResourceVtbl;

SG_INTERFACE ISGResource
{
    ISGResourceVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGBuffer : public ISGResource
{
public:
    virtual SG_RESULT SG_CALL GetDesc(
        SG_BUFFER_DESC* pOutDesc) = 0;

    virtual SG_RESULT SG_CALL Map(
        void** ppData) = 0;

    virtual void SG_CALL Unmap() = 0;
};

#else
typedef struct ISGBufferVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGBuffer* pThis);

    SgU32 (SG_CALL *Release)(
        ISGBuffer* pThis);

    // ISGResource
    SG_RESOURCE_TYPE (SG_CALL *GetType)(
        ISGBuffer* pThis);

    void (SG_CALL *SetName)(
        ISGBuffer* pThis,
        char const* pName);

    void (SG_CALL *GetName)(
        ISGBuffer* pThis,
        SgU32* pStrLen,
        char* pName);

    // ISGBuffer
    SG_RESULT (SG_CALL *GetDesc)(
        ISGBuffer* pThis,
        SG_BUFFER_DESC* pOutDesc);

    SG_RESULT (SG_CALL *Map)(
        ISGBuffer* pThis,
        void** ppData);

    void (SG_CALL *Unmap)(
        ISGBuffer* pThis);
} ISGBufferVtbl;

SG_INTERFACE ISGBuffer
{
    ISGBufferVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGSubresource : public ISGObject
{
public:
    virtual SG_RESULT   SG_CALL Map(
        SG_MAPPED_SUBRESOURCE* pMappedSubresource) = 0;

    virtual void        SG_CALL Unmap() = 0;
};

#else
typedef struct ISGSubresourceVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGSubresource* pThis);

    SgU32 (SG_CALL *Release)(
        ISGSubresource* pThis);

    // ISGSubresource
    SG_RESULT (SG_CALL *Map)(
        ISGSubresource* pThis,
        SG_MAPPED_SUBRESOURCE* pMappedSubresource);

    void (SG_CALL *Unmap)(
        ISGSubresource* pThis);

} ISGSubresourceVtbl;

SG_INTERFACE ISGSubresource
{
    ISGSubresourceVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGTexture : public ISGResource
{
public:
    virtual SG_RESULT SG_CALL GetDesc(
        SG_TEXTURE_DESC* pOutDesc) = 0;

    virtual SG_RESULT SG_CALL GetSubresourceInfo(
        SG_SUBRESOURCE_INFO* pOutInfo) = 0;

    virtual SG_RESULT SG_CALL GetSubresource(
        SgU32 mipSlice,
        SgU32 arraySlice,
        SgU32 planeSlice,
        ISGSubresource** ppSubresource) = 0;
};

#else
typedef struct ISGTextureVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGTexture* pThis);

    SgU32 (SG_CALL *Release)(
        ISGTexture* pThis);

    // ISGResource
    SG_RESOURCE_TYPE (SG_CALL *GetType)(
        ISGTexture* pThis);

    void (SG_CALL *SetName)(
        ISGTexture* pThis,
        char const* pName);

    void (SG_CALL *GetName)(
        ISGTexture* pThis,
        SgU32* pStrLen,
        char* pName);

    // ISGTexture
    SG_RESULT (SG_CALL *GetDesc)(
        ISGTexture* pThis,
        SG_TEXTURE_DESC* pOutDesc);

    SG_RESULT (SG_CALL *GetSubresourceInfo)(
        ISGTexture* pThis,
        SG_SUBRESOURCE_INFO* pOutInfo);

    SG_RESULT (SG_CALL *GetSubresource)(
        ISGTexture* pThis,
        SgU32 mipSlice,
        SgU32 arraySlice,
        SgU32 planeSlice,
        ISGSubresource** ppSubresource);

} ISGTextureVtbl;

SG_INTERFACE ISGTexture
{
    ISGTextureVtbl* pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
