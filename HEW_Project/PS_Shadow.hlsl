struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//// グレースケール変換
	////float3 gray = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
	//float3 gray = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
	//color = float4(gray.r, gray.g, gray.b, 1.0f);
	
	// テクスチャから色を取得
	float4 color = tex.Sample(samp, pin.uv);
	
	// RGB値を平均化
	float gray = (color.r + color.g + color.b) / 3.0f;
	color = float4(gray, gray, gray, 1.0f);
	
	//return tex.Sample(samp, pin.uv) * pin.color;
	return color;
}