
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

cbuffer JulieSetParameters : register(b0)
{
    float2 InvTextureSize;
    float Angle;
};

RWTexture2D<float4> OutTexture : register(u0);

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float2 vPos = float2(
        DTid.x * 2.0f * InvTextureSize.x - 1.0f,
        DTid.y * 2.0f * InvTextureSize.y - 1.0f
    );

    const float Radius = 1.0f;
    const float I = 500.0f;

    const float cosKoef = 0.7885f * cos(Angle);
    const float sinKoef = sin(Angle);

    float2 z = vPos * Radius;
    float n = 0.0;

    for (int i = I; i > 0; i--)
    {
        if (z.x * z.x + z.y * z.y > 4.0f)
        {
            n = i / I;
            break;
        }

        z = float2((z.x * z.x - z.y * z.y) + cosKoef, (2.0f * z.x * z.y) - sinKoef);
    }

    OutTexture[DTid.xy] = float4(0.5f - cos(n * 75.0f) / 2.0, 0.5f - cos(n * 120.0f) / 2.0f, 0.5f - cos(n * 165.0f) / 2.0f, 1.0f);
}
