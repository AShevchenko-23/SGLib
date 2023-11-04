
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

cbuffer CommonParameters : register(b0)
{
    matrix WorldTransform;
    matrix ViewProjection;
};

VS_OUTPUT VS(VS_INPUT input, uint instanceID : SV_InstanceID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    int xIndex = (instanceID % 8);
    int yIndex = ((instanceID / 8) % 8);
    int zIndex = (instanceID / 64);
    float3 position = input.Position * 0.1f;
    position += (float3(xIndex, yIndex, zIndex) - 3.5f) * 0.25f;

    output.Position = mul( WorldTransform, float4(position, 1) );
    output.Position = mul( ViewProjection, output.Position );
    output.TextureUV = input.TextureUV;
    return output;
}
