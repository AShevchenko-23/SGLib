
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

#include "CoreTypes.h"
#include "Format.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGShaderResourceView);
SG_DECLARE_CLASS(ISGUnorderedAccessView);
SG_DECLARE_CLASS(ISGRenderTargetView);
SG_DECLARE_CLASS(ISGDepthStencilView);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_SRV_TYPE
{
    SG_SRV_TYPE_STRUCTURED_BUFFER = 0,
    SG_SRV_TYPE_TYPED_BUFFER = 1,
    SG_SRV_TYPE_BYTE_ADDRESS_BUFFER = 2,
    SG_SRV_TYPE_TEXTURE = 3,
    SG_SRV_TYPE_TEXTURE_CUBE = 4
} SG_SRV_TYPE;

typedef enum SG_UAV_TYPE
{
    SG_UAV_TYPE_STRUCTURED_BUFFER = 0,
    SG_UAV_TYPE_TYPED_BUFFER = 1,
    SG_UAV_TYPE_BYTE_ADDRESS_BUFFER = 2,
    SG_UAV_TYPE_TEXTURE = 3,
} SG_UAV_TYPE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SG_STRUCTURED_BUFFER_VIEW_DESC
{
    SgU32 FirstElement;
    SgU32 ElementCount;
    SgU32 Stride;
} SG_STRUCTURED_BUFFER_VIEW_DESC;

typedef struct SG_TYPED_BUFFER_VIEW_DESC
{
    SgU32 FirstElement;
    SgU32 ElementCount;
    SG_FORMAT Format;
} SG_TYPED_BUFFER_VIEW_DESC;

typedef struct SG_BYTE_ADDRESS_BUFFER_VIEW_DESC
{
    SgU32 First32BitElement;
    SgU32 Num32BitElements;
} SG_BYTE_ADDRESS_BUFFER_VIEW_DESC;

typedef struct SG_TEXTURE_VIEW_DESC
{
    SG_FORMAT Format;
    SgU32 MostDetailedMip;
    SgU32 MipLevels;
    SgU32 FirstArraySlice;
    SgU32 ArraySize;
    SgU8  PlaneSlice;
} SG_TEXTURE_VIEW_DESC;

typedef struct SG_TEXTURE_CUBE_VIEW_DESC
{
    SG_FORMAT Format;
    SgU32 MostDetailedMip;
    SgU32 MipLevels;
    SgU32 FirstFace;
    SgU32 NumCubes;
} SG_TEXTURE_CUBE_VIEW_DESC;

typedef struct SG_SHADER_RESOURCE_VIEW_DESC
{
    SG_SRV_TYPE Type;
    union
    {
        SG_STRUCTURED_BUFFER_VIEW_DESC StructuredBuffer;
        SG_TYPED_BUFFER_VIEW_DESC TypedBuffer;
        SG_BYTE_ADDRESS_BUFFER_VIEW_DESC ByteAddressBuffer;
        SG_TEXTURE_VIEW_DESC Texture;
        SG_TEXTURE_CUBE_VIEW_DESC TextureCube;
    };
} SG_SHADER_RESOURCE_VIEW_DESC;

typedef struct SG_RW_TEXTURE_VIEW_DESC
{
    SG_FORMAT Format;
    SgU32 MipSlice;
    SgU32 FirstArraySlice;
    SgU32 ArraySize;
    SgU8  PlaneSlice;
} SG_RW_TEXTURE_VIEW_DESC;

typedef struct SG_UNORDERED_ACCESS_VIEW_DESC
{
    SG_UAV_TYPE Type;
    union
    {
        SG_STRUCTURED_BUFFER_VIEW_DESC StructuredBuffer;
        SG_TYPED_BUFFER_VIEW_DESC TypedBuffer;
        SG_BYTE_ADDRESS_BUFFER_VIEW_DESC ByteAddressBuffer;
        SG_RW_TEXTURE_VIEW_DESC Texture;
    };
} SG_UNORDERED_ACCESS_VIEW_DESC;

typedef struct SG_RENDER_TARGET_VIEW_DESC
{
    SG_FORMAT Format;
    SgU32    MipSlice;
    SgU32    FirstArraySlice;
    SgU32    ArraySize;
    SgU8     PlaneSlice;
} SG_RENDER_TARGET_VIEW_DESC;

typedef struct SG_DEPTH_STENCIL_VIEW_DESC
{
    SG_FORMAT Format;
    SgU32 MipSlice;
    SgU32 FirstArraySlice;
    SgU32 ArraySize;
} SG_DEPTH_STENCIL_VIEW_DESC;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE ISGShaderResourceView : public ISGObject
{
public:
};

#else
typedef struct ISGShaderResourceViewVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGShaderResourceView* pThis);

    SgU32 (SG_CALL *Release)(
        ISGShaderResourceView* pThis);

} ISGShaderResourceViewVtbl;

SG_INTERFACE ISGShaderResourceView
{
    ISGShaderResourceViewVtbl * pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE ISGUnorderedAccessView : public ISGObject
{
public:
};

#else
typedef struct ISGUnorderedAccessViewVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGUnorderedAccessView* pThis);

    SgU32 (SG_CALL *Release)(
        ISGUnorderedAccessView* pThis);

} ISGUnorderedAccessViewVtbl;

SG_INTERFACE ISGUnorderedAccessView
{
    ISGUnorderedAccessViewVtbl * pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE ISGRenderTargetView : public ISGObject
{
public:
};

#else
typedef struct ISGRenderTargetViewVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGRenderTargetView* pThis);

    SgU32 (SG_CALL *Release)(
        ISGRenderTargetView* pThis);

} ISGRenderTargetViewVtbl;

SG_INTERFACE ISGRenderTargetView
{
    ISGRenderTargetViewVtbl * pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE ISGDepthStencilView : public ISGObject
{
public:
};

#else
typedef struct ISGDepthStencilViewVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGDepthStencilView* pThis);

    SgU32 (SG_CALL *Release)(
        ISGDepthStencilView* pThis);

} ISGDepthStencilViewVtbl;

SG_INTERFACE ISGDepthStencilView
{
    ISGDepthStencilViewVtbl * pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
