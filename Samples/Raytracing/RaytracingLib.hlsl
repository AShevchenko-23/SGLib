
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

#define MAX_REFLECT_RAY_LENGTH 10.0f
#define MAX_REFLECT_RAY_FADE 7.0f
typedef BuiltInTriangleIntersectionAttributes MyAttributes;

struct Vertex
{
    float3 Position;
    float2 TextureUV;
};

cbuffer RayGenConstantBuffer                        : register(b0)
{
    float4x4 ProjToWorldMatrix;
    float4 CameraPosition;
}

StructuredBuffer<Vertex>        VertexBuffer        : register(t0);
StructuredBuffer<uint>          IndexBuffer         : register(t1);
Texture2D                       SimpleTexture       : register(t2);
Texture2D                       AlphaTestTexture    : register(t3);
RaytracingAccelerationStructure Scene               : register(t4);

RWTexture2D<float4>             OutTexture          : register(u0);

SamplerState                    Sampler             : register(s0);

struct PayloadData
{
    float3 color;
    float rayLength;
};

struct Ray
{
    float3 origin;
    float3 direction;
};

inline Ray GenerateCameraRay(uint2 index, in float3 cameraPosition, in float4x4 projectionToWorld)
{
    float2 xy = (float2)index + 0.5f; // center in the middle of the pixel.
    float2 screenPos = xy / DispatchRaysDimensions().xy * 2.0 - 1.0;

    // Invert Y for DirectX-style coordinates.
    screenPos.y = -screenPos.y;

    // Unproject the pixel coordinate into a world positon.
    float4 world = mul(projectionToWorld, float4(screenPos, 0, 1));
    world.xyz /= world.w;

    Ray ray;
    ray.origin = cameraPosition;
    ray.direction = normalize(world.xyz - ray.origin);

    return ray;
}

[shader("raygeneration")]
void RayGen()
{
    Ray ray = GenerateCameraRay(DispatchRaysIndex().xy, CameraPosition.xyz, ProjToWorldMatrix);

    // Cast rays
    RayDesc myRay = { ray.origin,
        0.01f,
        ray.direction,
        150.0f };

    PayloadData payload = (PayloadData)0;
    TraceRay(Scene, RAY_FLAG_NONE, ~0, 0, 1, 0, myRay, payload);

    OutTexture[DispatchRaysIndex().xy] = float4(payload.color, 1);
}

[shader("closesthit")]
void ClosestHitFloor(inout PayloadData payload : SV_RayPayload, in MyAttributes attr : SV_IntersectionAttributes)
{
    float3 hitPosition = WorldRayOrigin() + WorldRayDirection() * RayTCurrent();
    
    float3 floorNormal = float3(0, 1, 0);
    float3 reflectRay = reflect(WorldRayDirection(), floorNormal);

    RayDesc myRay = { hitPosition,
        0.01f,
        reflectRay,
        MAX_REFLECT_RAY_LENGTH };

    PayloadData reflectPayload = (PayloadData)0;
    TraceRay(Scene, RAY_FLAG_NONE, ~0, 0, 1, 0, myRay, reflectPayload);

    payload.color = lerp(float3(0.5f, 0.5f, 0.5f), reflectPayload.color, max(0.01f, (1.0f - saturate(reflectPayload.rayLength / MAX_REFLECT_RAY_FADE)) * 0.7f) );
    payload.rayLength = RayTCurrent();
}

[shader("closesthit")]
void ClosestHitBary(inout PayloadData payload : SV_RayPayload, in MyAttributes attr : SV_IntersectionAttributes)
{
    payload.color = float3(1.0 - attr.barycentrics.x - attr.barycentrics.y, attr.barycentrics.x, attr.barycentrics.y);
    payload.rayLength = RayTCurrent();
}

void ReadIndices(uint primitiveIndex, out uint index0, out uint index1, out uint index2)
{
    uint firstIndex = primitiveIndex * 3;

    // As index buffer is in U16 format, need to make some tricks to get triangle indices
    bool odd = firstIndex & 1;
    uint ibIndex = firstIndex >> 1;

    uint value0 = IndexBuffer[ibIndex];
    uint value1 = IndexBuffer[ibIndex + 1];

    if (odd)
    {
        index0 = value0 >> 16;
        index1 = value1 & 0xFFFF;
        index2 = value1 >> 16;
    }
    else
    {
        index0 = value0 & 0xFFFF;
        index1 = value0 >> 16;
        index2 = value1 & 0xFFFF;
    }
}

#define INTERPOLATE_VECTORS(bary, v1, v2, v3) v1 + bary.x * (v2 - v1) + bary.y * (v3 - v1)

void GetVertexData(uint primitiveIndex, float2 barycentrics, out float3 position, out float2 textureUV)
{
    uint index0 = 0, index1 = 0, index2 = 0;
    ReadIndices(primitiveIndex, index0, index1, index2);

    Vertex v0, v1, v2;
    v0 = VertexBuffer[index0];
    v1 = VertexBuffer[index1];
    v2 = VertexBuffer[index2];

    position = INTERPOLATE_VECTORS(barycentrics, v0.Position, v1.Position, v2.Position);
    textureUV = INTERPOLATE_VECTORS(barycentrics, v0.TextureUV, v1.TextureUV, v2.TextureUV);
}

[shader("closesthit")]
void ClosestHitSimpleTexture(inout PayloadData payload : SV_RayPayload, in MyAttributes attr : SV_IntersectionAttributes)
{
    float3 position = 0;
    float2 uv = 0;
    GetVertexData(PrimitiveIndex(), attr.barycentrics, position, uv);

    payload.color = SimpleTexture.SampleLevel(Sampler, uv, 0).rgb;
    payload.rayLength = RayTCurrent();
}

[shader("anyhit")]
void AnyHitAlphaTest(inout PayloadData payload : SV_RayPayload, in MyAttributes attr : SV_IntersectionAttributes)
{
    float3 position = 0;
    float2 uv = 0;
    GetVertexData(PrimitiveIndex(), attr.barycentrics, position, uv);

    float3 color = AlphaTestTexture.SampleLevel(Sampler, uv, 0).rgb;

    if (color.r >= 0.5f)
        IgnoreHit();

    payload.color = color;
    payload.rayLength = RayTCurrent();
}

[shader("miss")]
void Miss(inout PayloadData payload : SV_RayPayload)
{
    payload.color = float3(0.8f, 0.9f, 1.0f);
    payload.rayLength = MAX_REFLECT_RAY_LENGTH * 0.5f;
}