
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

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGSampler);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_FILTER_TYPE
{
    SG_FILTER_TYPE_POINT = 0,
    SG_FILTER_TYPE_LINEAR = 1,
} SG_FILTER_TYPE;

typedef enum SG_FILTER_REDUCTION
{
    SG_FILTER_REDUCTION_STANDART = 0,
    SG_FILTER_REDUCTION_COMPARISON = 1,
    SG_FILTER_REDUCTION_MINIMUM = 2,
    SG_FILTER_REDUCTION_MAXIMUMM = 3,
} SG_FILTER_REDUCTION;

typedef enum SG_FILTER
{
    SG_FILTER_MIN_MAG_MIP_POINT = 0,
    SG_FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
    SG_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
    SG_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
    SG_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
    SG_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
    SG_FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
    SG_FILTER_MIN_MAG_MIP_LINEAR = 0x15,
    SG_FILTER_ANISOTROPIC = 0x55,
    SG_FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
    SG_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
    SG_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
    SG_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
    SG_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
    SG_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
    SG_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
    SG_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
    SG_FILTER_COMPARISON_ANISOTROPIC = 0xD5,
    SG_FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
    SG_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
    SG_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
    SG_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
    SG_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
    SG_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
    SG_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
    SG_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
    SG_FILTER_MINIMUM_ANISOTROPIC = 0x155,
    SG_FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
    SG_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
    SG_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
    SG_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
    SG_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
    SG_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
    SG_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
    SG_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
    SG_FILTER_MAXIMUM_ANISOTROPIC = 0x1D5
} SG_FILTER;

typedef enum SG_TEXTURE_ADDRESS_MODE
{
    SG_TEXTURE_ADDRESS_MODE_WRAP = 0,
    SG_TEXTURE_ADDRESS_MODE_MIRROR = 1,
    SG_TEXTURE_ADDRESS_MODE_CLAMP = 2,
    SG_TEXTURE_ADDRESS_MODE_BORDER = 3,
    SG_TEXTURE_ADDRESS_MODE_MIRROR_ONCE = 4,
} SG_TEXTURE_ADDRESS_MODE;

typedef enum SG_COMPARISON_FUNC
{
    SG_COMPARISON_FUNC_NEVER = 0,
    SG_COMPARISON_FUNC_LESS = 1,
    SG_COMPARISON_FUNC_EQUAL = 2,
    SG_COMPARISON_FUNC_LESS_EQUAL = 3,
    SG_COMPARISON_FUNC_GREATER = 4,
    SG_COMPARISON_FUNC_NOT_EQUAL = 5,
    SG_COMPARISON_FUNC_GREATER_EQUAL = 6,
    SG_COMPARISON_FUNC_ALWAYS = 7,
} SG_COMPARISON_FUNC;


#define SG_FILTER_REDUCTION_TYPE_MASK   ( 0x3 )
#define SG_FILTER_REDUCTION_TYPE_SHIFT  ( 7 )
#define SG_FILTER_TYPE_MASK             ( 0x3 )
#define SG_MIN_FILTER_SHIFT             ( 4 )
#define SG_MAG_FILTER_SHIFT             ( 2 )
#define SG_MIP_FILTER_SHIFT             ( 0 )
#define SG_ANISOTROPIC_FILTERING_BIT    ( 0x40 )

#define SG_ENCODE_BASIC_FILTER( min, mag, mip, reduction )                                                  \
                                   ( ( SG_FILTER ) (                                                        \
                                   ( ( ( min ) & SG_FILTER_TYPE_MASK ) << SG_MIN_FILTER_SHIFT ) |   \
                                   ( ( ( mag ) & SG_FILTER_TYPE_MASK ) << SG_MAG_FILTER_SHIFT ) |   \
                                   ( ( ( mip ) & SG_FILTER_TYPE_MASK ) << SG_MIP_FILTER_SHIFT ) |   \
                                   ( ( ( reduction ) & SG_FILTER_REDUCTION_TYPE_MASK ) << SG_FILTER_REDUCTION_TYPE_SHIFT ) ) )

#define SG_ENCODE_ANISOTROPIC_FILTER( reduction )                                                       \
                                         ( ( SG_FILTER ) (                                              \
                                         SG_ANISOTROPIC_FILTERING_BIT |                                 \
                                         SG_ENCODE_BASIC_FILTER( SG_FILTER_TYPE_LINEAR,                 \
                                                                 SG_FILTER_TYPE_LINEAR,                 \
                                                                 SG_FILTER_TYPE_LINEAR,                 \
                                                                 reduction ) ) )

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SG_SAMPLER_DESC
{
    SG_FILTER Filter;
    SG_TEXTURE_ADDRESS_MODE AddressU;
    SG_TEXTURE_ADDRESS_MODE AddressV;
    SG_TEXTURE_ADDRESS_MODE AddressW;
    float               MipLODBias;
    SgU32               MaxAnisotropy;
    SG_COMPARISON_FUNC  ComparisonFunc;
    float               BorderColor[4];
    float               MinLOD;
    float               MaxLOD;
} SG_SAMPLER_DESC;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE SG_API ISGSampler : public ISGObject
{
public:
};

#else

typedef struct ISGSamplerVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGSampler* pThis);

    SgU32 (SG_CALL *Release)(
        ISGSampler* pThis);
} ISGSamplerVtbl;

SG_INTERFACE ISGSampler
{
    ISGSamplerVtbl * pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
