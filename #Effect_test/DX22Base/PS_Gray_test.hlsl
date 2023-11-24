struct PS_IN
{
    float4 pos : POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};


cbuffer Param : register(b0)
{
    float4x4 mat;
    float4 color;
}

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
    float4 col = tex.Sample(samp, pin.uv);
    float4 graycol = (col.r * col.r + col.g * col.g + col.b * col.b) / 3;
    return graycol * color;
}