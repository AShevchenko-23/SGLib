
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

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return Texture.Sample(Sampler, input.TextureUV);
}
