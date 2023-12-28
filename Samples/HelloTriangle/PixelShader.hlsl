struct PS_DATA
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR0;
};

float4 main(PS_DATA dat) : SV_TARGET
{
    return float4(dat.Color, 1.0f);
}