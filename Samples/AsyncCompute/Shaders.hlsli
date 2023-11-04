
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

struct VS_INPUT
{
    float3 Position   : POSITION;
    float2 TextureUV  : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Position   : SV_POSITION;
    float2 TextureUV  : TEXCOORD0;
};
