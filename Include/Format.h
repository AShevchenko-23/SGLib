
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

typedef enum SG_FORMAT
{
    SG_FORMAT_UNKNOWN                     = 0,
    SG_FORMAT_R32G32B32A32_TYPELESS       = 1,
    SG_FORMAT_R32G32B32A32_FLOAT          = 2,
    SG_FORMAT_R32G32B32A32_UINT           = 3,
    SG_FORMAT_R32G32B32A32_SINT           = 4,
    SG_FORMAT_R32G32B32_TYPELESS          = 5,
    SG_FORMAT_R32G32B32_FLOAT             = 6,
    SG_FORMAT_R32G32B32_UINT              = 7,
    SG_FORMAT_R32G32B32_SINT              = 8,
    SG_FORMAT_R16G16B16A16_TYPELESS       = 9,
    SG_FORMAT_R16G16B16A16_FLOAT          = 10,
    SG_FORMAT_R16G16B16A16_UNORM          = 11,
    SG_FORMAT_R16G16B16A16_UINT           = 12,
    SG_FORMAT_R16G16B16A16_SNORM          = 13,
    SG_FORMAT_R16G16B16A16_SINT           = 14,
    SG_FORMAT_R32G32_TYPELESS             = 15,
    SG_FORMAT_R32G32_FLOAT                = 16,
    SG_FORMAT_R32G32_UINT                 = 17,
    SG_FORMAT_R32G32_SINT                 = 18,
    SG_FORMAT_R32G8X24_TYPELESS           = 19,
    SG_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
    SG_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
    SG_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
    SG_FORMAT_R10G10B10A2_TYPELESS        = 23,
    SG_FORMAT_R10G10B10A2_UNORM           = 24,
    SG_FORMAT_R10G10B10A2_UINT            = 25,
    SG_FORMAT_R11G11B10_FLOAT             = 26,
    SG_FORMAT_R8G8B8A8_TYPELESS           = 27,
    SG_FORMAT_R8G8B8A8_UNORM              = 28,
    SG_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
    SG_FORMAT_R8G8B8A8_UINT               = 30,
    SG_FORMAT_R8G8B8A8_SNORM              = 31,
    SG_FORMAT_R8G8B8A8_SINT               = 32,
    SG_FORMAT_R16G16_TYPELESS             = 33,
    SG_FORMAT_R16G16_FLOAT                = 34,
    SG_FORMAT_R16G16_UNORM                = 35,
    SG_FORMAT_R16G16_UINT                 = 36,
    SG_FORMAT_R16G16_SNORM                = 37,
    SG_FORMAT_R16G16_SINT                 = 38,
    SG_FORMAT_R32_TYPELESS                = 39,
    SG_FORMAT_D32_FLOAT                   = 40,
    SG_FORMAT_R32_FLOAT                   = 41,
    SG_FORMAT_R32_UINT                    = 42,
    SG_FORMAT_R32_SINT                    = 43,
    SG_FORMAT_R24G8_TYPELESS              = 44,
    SG_FORMAT_D24_UNORM_S8_UINT           = 45,
    SG_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
    SG_FORMAT_X24_TYPELESS_G8_UINT        = 47,
    SG_FORMAT_R8G8_TYPELESS               = 48,
    SG_FORMAT_R8G8_UNORM                  = 49,
    SG_FORMAT_R8G8_UINT                   = 50,
    SG_FORMAT_R8G8_SNORM                  = 51,
    SG_FORMAT_R8G8_SINT                   = 52,
    SG_FORMAT_R16_TYPELESS                = 53,
    SG_FORMAT_R16_FLOAT                   = 54,
    SG_FORMAT_D16_UNORM                   = 55,
    SG_FORMAT_R16_UNORM                   = 56,
    SG_FORMAT_R16_UINT                    = 57,
    SG_FORMAT_R16_SNORM                   = 58,
    SG_FORMAT_R16_SINT                    = 59,
    SG_FORMAT_R8_TYPELESS                 = 60,
    SG_FORMAT_R8_UNORM                    = 61,
    SG_FORMAT_R8_UINT                     = 62,
    SG_FORMAT_R8_SNORM                    = 63,
    SG_FORMAT_R8_SINT                     = 64,
    SG_FORMAT_A8_UNORM                    = 65,
    SG_FORMAT_R1_UNORM                    = 66,
    SG_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
    SG_FORMAT_R8G8_B8G8_UNORM             = 68,
    SG_FORMAT_G8R8_G8B8_UNORM             = 69,
    SG_FORMAT_BC1_TYPELESS                = 70,
    SG_FORMAT_BC1_UNORM                   = 71,
    SG_FORMAT_BC1_UNORM_SRGB              = 72,
    SG_FORMAT_BC2_TYPELESS                = 73,
    SG_FORMAT_BC2_UNORM                   = 74,
    SG_FORMAT_BC2_UNORM_SRGB              = 75,
    SG_FORMAT_BC3_TYPELESS                = 76,
    SG_FORMAT_BC3_UNORM                   = 77,
    SG_FORMAT_BC3_UNORM_SRGB              = 78,
    SG_FORMAT_BC4_TYPELESS                = 79,
    SG_FORMAT_BC4_UNORM                   = 80,
    SG_FORMAT_BC4_SNORM                   = 81,
    SG_FORMAT_BC5_TYPELESS                = 82,
    SG_FORMAT_BC5_UNORM                   = 83,
    SG_FORMAT_BC5_SNORM                   = 84,
    SG_FORMAT_B5G6R5_UNORM                = 85,
    SG_FORMAT_B5G5R5A1_UNORM              = 86,
    SG_FORMAT_B8G8R8A8_UNORM              = 87,
    SG_FORMAT_B8G8R8X8_UNORM              = 88,
    SG_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
    SG_FORMAT_B8G8R8A8_TYPELESS           = 90,
    SG_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
    SG_FORMAT_B8G8R8X8_TYPELESS           = 92,
    SG_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
    SG_FORMAT_BC6H_TYPELESS               = 94,
    SG_FORMAT_BC6H_UF16                   = 95,
    SG_FORMAT_BC6H_SF16                   = 96,
    SG_FORMAT_BC7_TYPELESS                = 97,
    SG_FORMAT_BC7_UNORM                   = 98,
    SG_FORMAT_BC7_UNORM_SRGB              = 99,
} SG_FORMAT;
