struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
	// テクスチャからピクセルの色を取得
	float4 color = tex.Sample(samp, pin.uv);
	// グレースケールに変換
	float gray = dot(color.rgb, float3(0.299, 0.587, 0.114));
	
	return float4(gray, gray, gray, color.a);

}