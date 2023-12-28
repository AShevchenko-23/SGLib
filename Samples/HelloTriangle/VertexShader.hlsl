struct Vertex
{
    float3 Pos : POSITION0;
    float3 Color : COLOR0;
};

struct PS_DATA
{
    float4 Pos : SV_POSITION;
    float3 Color : COLOR0;
};

PS_DATA main(Vertex input)
{
    PS_DATA output = (PS_DATA) 0;
    output.Pos = float4(input.Pos, 1.0f);
    output.Color = input.Color;

    return output;
}