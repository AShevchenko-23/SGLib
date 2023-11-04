
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

#include "Shaders.hlsli"

float4 PS(VS_OUTPUT input, uint primID : SV_PrimitiveID) : SV_TARGET
{
    uint faceIndex = primID / 2;
    return Texture.Sample(Sampler, float3(input.TextureUV, faceIndex));
}
