struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};
Texture2D tex : register(t0);
SamplerState samp : register(s0);

#define FILTER_SIZE 5
#define SIGMA_D 3.0f
#define SIGMA_R 0.1f

float4 main(PS_IN pin) : SV_TARGET
{
	//float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//// グレースケール変換
	////float3 gray = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
	//float3 gray = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));
	//color = float4(gray.r, gray.g, gray.b, 1.0f);
	
	// テクスチャから色を取得
	float4 color = tex.Sample(samp, pin.uv);
	
	//// RGB値を平均化
	float gray = (color.r + color.g + color.b) / 3.0f;
	////color = float4(gray, gray, gray, 1.0f);
	
	// しきい値を設定
	float threshold = 0.8f;
	
	//// 2値化
	float4 binaryColor = (gray >= threshold) ? float4(1.0f, 1.0f, 1.0f, 1.0f) : float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//return gray;
	
	//return binaryColor;
	
	// 1/テクスチャのサイズ
	uint width, height;
	tex.GetDimensions(width, height);
	float2 offset = 1.0f / float2(width, height);
	
	// 入力画像の中心ピクセルの色
	float4 center = tex.Sample(samp, pin.uv);
	
	// フィルターの重みの合計
	float weightSum = 0.0f;
	
	// 出力色
	float4 output = 0.0f;
	
	// フィルター処理
	for (int i = -FILTER_SIZE; i <= FILTER_SIZE; i++)
	{
		for (int j = -FILTER_SIZE; j <= FILTER_SIZE; j++)
		{
			// フィルタ内のピクセルの座標
			float2 offset = float2(i, j) / float2(width, height);
			
			// フィルタ内のピクセルの色
			float4 neighbor = tex.Sample(samp, pin.uv + offset);
			
			// 距離に基づく重み(空間的な距離)
			float weightD = exp(-0.5f * (i * i + j * j) / (SIGMA_D * SIGMA_D));
			
			// 色差に基づく重み(輝度値の差)
			float weightR = exp(-0.5f * dot(neighbor - center, neighbor - center) / (SIGMA_R * SIGMA_R));
			
			// 重みの積
			float weight = weightD * weightR;
			
			// 重みの合計に加算
			weightSum += weight;
			
			// 出力色に加算
			output += weight * neighbor;

		}

	}
	
	// 重みの合計で割って正規化
	output /= weightSum;
	
	// グレースケール
	float4 grayScale = dot(output.rgb, float3(0.299f, 0.587f, 0.114f));
	
	
	// 2値化
	binaryColor = (grayScale >= threshold) ? float4(1.0f, 1.0f, 1.0f, 1.0f) : float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return binaryColor;
	//return grayScale;
}