#include "Shader.h"
#include <d3dcompiler.h>
#include <stdio.h>

#pragma comment(lib, "d3dcompiler.lib") // ライブラリのリンク

//----------
// 基本クラス
Shader::Shader(Kind kind)
	: m_kind(kind)
{
}
Shader::~Shader()
{
	std::vector<ID3D11Buffer*>::iterator it = m_pBuffers.begin();
	while (it != m_pBuffers.end())
	{
		SAFE_RELEASE((*it));
		++it;
	}
}
HRESULT Shader::Load(const char* pFileName)
{
	HRESULT hr = E_FAIL;

	// ファイルを読み込む
	FILE* fp;
	fopen_s(&fp, pFileName, "rb");	// バイナリモードで読み込み
	if (!fp) { return hr; }

	// ファイルのサイズを調べる
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);	// ファイルの末尾に移動
	fileSize = ftell(fp);	// ファイルポインタの位置を取得

	// メモリに読み込み
	fseek(fp, 0, SEEK_SET);	// ファイルの先頭に移動
	char* pData = new char[fileSize];	// ファイルサイズ分のメモリを確保
	fread(pData, fileSize, 1, fp);	// ファイルをメモリに読み込み
	fclose(fp);

	// シェーダー作成
	hr = Make(pData, fileSize);	// シェーダー作成
	
	// 終了処理
	if (pData) { delete[] pData; }
	return hr;
}
HRESULT Shader::Compile(const char *pCode)
{
	static const char *pTargetList[] = 	// シェーダーターゲット
	{
		"vs_5_0",	// 頂点シェーダー
		"ps_5_0"	// ピクセルシェーダー
	};

	HRESULT hr;
	ID3DBlob *pBlob;	// コンパイル後のデータ
	ID3DBlob *error;	// エラー情報
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;	// コンパイルフラグ: デバッグ情報あり、最適化なし
	hr = D3DCompile(pCode, strlen(pCode), nullptr, nullptr, nullptr,	// コンパイル
		"main", pTargetList[m_kind], compileFlag, 0, &pBlob, &error);	
	// ↑ 引数(コード, コードサイズ, ファイル名, マクロ, インクルード, エントリポイント, ターゲット, コンパイルフラグ, エフェクトコンパイルフラグ, コンパイル後のデータ, エラー情報)
	//	 コード: pCode = コンパイルするシェーダーコード(HLSL)
	//	 コードサイズ: strlen(pCode) = コードのサイズ = 文字列の長さを取得
	//	 ファイル名: nullptr = ファイル名なし
	//	 マクロ: nullptr = マクロなし
	//	 インクルード: nullptr = インクルードなし
	//	 エントリポイント: "main" = エントリポイントの関数名
	//	 ターゲット: pTargetList[m_kind] = シェーダーターゲット = 頂点シェーダー or ピクセルシェーダー
	//	 コンパイルフラグ: compileFlag = コンパイルフラグ = デバッグ情報あり、最適化なし
	//	 エフェクトコンパイルフラグ: 0 = デフォルト	
	//	 コンパイル後のデータ: &pBlob = コンパイル後のデータを格納する変数のアドレス
	//	 エラー情報: &error = エラー情報を格納する変数のアドレス
	if (FAILED(hr)) { return hr; }

	// シェーダ作成
	hr = Make(pBlob->GetBufferPointer(), (UINT)pBlob->GetBufferSize());	// シェーダー作成 = Make関数にコンパイル後のデータを渡す
	SAFE_RELEASE(pBlob);	// コンパイル後のデータを解放
	SAFE_RELEASE(error);	// エラー情報を解放
	return hr;
}

void Shader::WriteBuffer(UINT slot, void* pData)
{
	if(slot < m_pBuffers.size())	// バッファの数より小さい場合
		GetContext()->UpdateSubresource(m_pBuffers[slot], 0, nullptr, pData, 0, 0);	// バッファに書き込み
		// ↑ 引数(バッファ, サブリソース, 領域, データ, データサイズ, データのオフセット)
		//	 バッファ: m_pBuffers[slot] = バッファのポインタ
		//	 サブリソース: 0 = デフォルト
		//	 領域: nullptr = デフォルト
		//	 データ: pData = 書き込むデータ
		//	 データサイズ: 0 = デフォルト
		//	 データのオフセット: 0 = デフォルト
}
void Shader::SetTexture(UINT slot, Texture* tex)
{
	if (!tex || slot >= m_pTextures.size()) { return; }	// テクスチャがない or テクスチャの数より大きい場合
	ID3D11ShaderResourceView* pTex = tex->GetResource();	// テクスチャのリソースを取得
	m_pTextures[slot] = pTex;	// テクスチャのリソースを格納
	switch (m_kind)	// シェーダーの種類別に処理
	{
	case Vertex:	GetContext()->VSSetShaderResources(slot, 1, &pTex); break;	// 頂点シェーダーの場合
	case Pixel:		GetContext()->PSSetShaderResources(slot, 1, &pTex); break;	// ピクセルシェーダーの場合
	}
}

HRESULT Shader::Make(void* pData, UINT size)	// シェーダー作成
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();	// デバイス取得

	// 解析用のリフレクション作成
	ID3D11ShaderReflection* pReflection;	// シェーダーリフレクション
	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));	// シェーダーリフレクションを作成
	// ↑ 引数(データ, データサイズ, インターフェースID, インターフェース)
	//	 データ: pData = シェーダーコード
	//	 データサイズ: size = シェーダーコードのサイズ
	//	 インターフェースID: IID_PPV_ARGS(&pReflection) = インターフェースID = シェーダーリフレクションのID
	if (FAILED(hr)) { return hr; }

	// 定数バッファ作成
	D3D11_SHADER_DESC shaderDesc;	// シェーダーの設定
	pReflection->GetDesc(&shaderDesc);	// シェーダーの設定を取得
	m_pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);	// バッファの数を設定
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)	// バッファの数だけ繰り返す
	{
		// シェーダーの定数バッファの情報を取得
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;	// シェーダーの定数バッファの情報
		ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);	// シェーダーの定数バッファを取得
		cbuf->GetDesc(&shaderBufDesc);	// シェーダーの定数バッファの情報を取得

		// 作成するバッファの情報
		D3D11_BUFFER_DESC bufDesc = {};	// バッファの情報
		bufDesc.ByteWidth = shaderBufDesc.Size;	// バッファのサイズ
		bufDesc.Usage = D3D11_USAGE_DEFAULT;	// 使用方法: D3D11_USAGE_DEFAULT = GPUが読み書き可能
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// バインドフラグ: D3D11_BIND_CONSTANT_BUFFER = 定数バッファとしてバインド

		// バッファの作成
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &m_pBuffers[i]);	// バッファを作成
		if (FAILED(hr)) { return hr; }	// 失敗した場合は終了
	}
	// テクスチャ領域作成
	m_pTextures.resize(shaderDesc.TextureNormalInstructions, nullptr);	// テクスチャの数を設定

	return MakeShader(pData, size);	// シェーダー作成
}

//----------
// 頂点シェーダ
VertexShader::VertexShader()
	: Shader(Shader::Vertex)
	, m_pVS(nullptr)
	, m_pInputLayout(nullptr)
{
}

VertexShader::~VertexShader()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVS);
}

void VertexShader::Bind(void)
{
	ID3D11DeviceContext* pContext =	GetContext();	// デバイスコンテキスト取得
	pContext->VSSetShader(m_pVS, NULL, 0);	// 頂点シェーダーを設定
	pContext->IASetInputLayout(m_pInputLayout);	// 入力レイアウトを設定
	for (int i = 0; i < m_pBuffers.size(); ++i)	// バッファの数だけ繰り返す
		pContext->VSSetConstantBuffers(i, 1, &m_pBuffers[i]);	// バッファを設定
	for (int i = 0; i < m_pTextures.size(); ++i)		// テクスチャの数だけ繰り返す
		pContext->VSSetShaderResources(i, 1, &m_pTextures[i]);	// テクスチャを設定
}

HRESULT VertexShader::MakeShader(void* pData, UINT size)
{
	HRESULT hr;
	ID3D11Device* pDevice = GetDevice();	// デバイス取得
	
	// シェーダー作成
	hr = pDevice->CreateVertexShader(pData, size, NULL, &m_pVS);	// 頂点シェーダーを作成
	if(FAILED(hr)) { return hr; }

	/*
	シェーダ作成時にシェーダリフレクションを通してインプットレイアウトを取得
	セマンティクスの配置などから識別子を作成
	識別子が登録済→再利用、なければ新規作成
	https://blog.techlab-xe.net/dxc-shader-reflection/
	*/
	// ↑ 参考サイト

	ID3D11ShaderReflection *pReflection;	// シェーダーリフレクション
	D3D11_SHADER_DESC shaderDesc;	// シェーダーの設定
	D3D11_INPUT_ELEMENT_DESC* pInputDesc;	// 入力レイアウトの設定
	D3D11_SIGNATURE_PARAMETER_DESC sigDesc;	// シグネチャの設定

	DXGI_FORMAT formats[][4] =	// フォーマット
	{
		{
			DXGI_FORMAT_R32_UINT,			// DXGI_FORMAT_R32_UINT = 32ビットの符号なし整数データ
			DXGI_FORMAT_R32G32_UINT,		// DXGI_FORMAT_R32G32_UINT = 32ビットの符号なし整数データ(2次元)
			DXGI_FORMAT_R32G32B32_UINT,		// DXGI_FORMAT_R32G32B32_UINT = 32ビットの符号なし整数データ(3次元)
			DXGI_FORMAT_R32G32B32A32_UINT,	// DXGI_FORMAT_R32G32B32A32_UINT = 32ビットの符号なし整数データ(4次元)
		}, {
			DXGI_FORMAT_R32_SINT,			// DXGI_FORMAT_R32_SINT = 32ビットの符号あり整数データ
			DXGI_FORMAT_R32G32_SINT,		// DXGI_FORMAT_R32G32_SINT = 32ビットの符号あり整数データ(2次元)
			DXGI_FORMAT_R32G32B32_SINT,		// DXGI_FORMAT_R32G32B32_SINT = 32ビットの符号あり整数データ(3次元)
			DXGI_FORMAT_R32G32B32A32_SINT,	// DXGI_FORMAT_R32G32B32A32_SINT = 32ビットの符号あり整数データ(4次元)
		}, {
			DXGI_FORMAT_R32_FLOAT,			// DXGI_FORMAT_R32_FLOAT = 32ビットの浮動小数点データ
			DXGI_FORMAT_R32G32_FLOAT,		// DXGI_FORMAT_R32G32_FLOAT = 32ビットの浮動小数点データ(2次元)
			DXGI_FORMAT_R32G32B32_FLOAT,	// DXGI_FORMAT_R32G32B32_FLOAT = 32ビットの浮動小数点データ(3次元)
			DXGI_FORMAT_R32G32B32A32_FLOAT,	// DXGI_FORMAT_R32G32B32A32_FLOAT = 32ビットの浮動小数点データ(4次元)
		},
	};

	hr = D3DReflect(pData, size, IID_PPV_ARGS(&pReflection));	// シェーダーリフレクションを作成
	if (FAILED(hr)) { return hr; }

	pReflection->GetDesc(&shaderDesc);	// シェーダーの設定を取得
	pInputDesc = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters];	// 入力レイアウトの設定を作成
	for(UINT i = 0; i < shaderDesc.InputParameters; ++ i)	// 入力レイアウトの設定の数だけ繰り返す
	{
		pReflection->GetInputParameterDesc(i, &sigDesc);	// シグネチャの設定を取得
		pInputDesc[i].SemanticName = sigDesc.SemanticName;	// セマンティクスの名前
		pInputDesc[i].SemanticIndex = sigDesc.SemanticIndex;	// セマンティクスのインデックス

		// http://marupeke296.com/TIPS_No17_Bit.html
		// ↑ 参考サイト ビット演算のあれこれ
		BYTE elementCount = sigDesc.Mask;	// マスク
		elementCount = (elementCount & 0x05) + ((elementCount >> 1) & 0x05);	// マスクの計算
		elementCount = (elementCount & 0x03) + ((elementCount >> 2) & 0x03);	// マスクの計算

		switch (sigDesc.ComponentType)	// コンポーネントタイプ
		{
			case D3D_REGISTER_COMPONENT_UINT32:	// D3D_REGISTER_COMPONENT_UINT32 = 32ビットの符号なし整数データ
				pInputDesc[i].Format = formats[0][elementCount - 1];	// フォーマット
				break;
			case D3D_REGISTER_COMPONENT_SINT32:	// D3D_REGISTER_COMPONENT_SINT32 = 32ビットの符号あり整数データ
				pInputDesc[i].Format = formats[1][elementCount - 1];	// フォーマット
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:	// D3D_REGISTER_COMPONENT_FLOAT32 = 32ビットの浮動小数点データ
				pInputDesc[i].Format = formats[2][elementCount - 1];	// フォーマット
				break;
		}
		pInputDesc[i].InputSlot = 0;	// 入力スロット
		pInputDesc[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;	// アライメントバイトオフセット
		pInputDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 入力スロットクラス: D3D11_INPUT_PER_VERTEX_DATA = 頂点ごとにデータを入力
		pInputDesc[i].InstanceDataStepRate = 0;	// インスタンスデータステップレート
	}

	hr = pDevice->CreateInputLayout(	// 入力レイアウトを作成
		pInputDesc, shaderDesc.InputParameters,	// 入力レイアウトの設定, 入力レイアウトの設定の数
		pData, size, &m_pInputLayout	// シェーダーコード, シェーダーコードのサイズ, 入力レイアウト
	);
	
	delete[] pInputDesc;	// 入力レイアウトの設定を解放
	return hr;	// 結果を返す
}

//----------
// ピクセルシェーダ
PixelShader::PixelShader()
	: Shader(Shader::Pixel)
	, m_pPS(nullptr)
{
}
PixelShader::~PixelShader()
{
	SAFE_RELEASE(m_pPS);
}
void PixelShader::Bind(void)
{
	ID3D11DeviceContext* pContext = GetContext();	// デバイスコンテキスト取得
	pContext->PSSetShader(m_pPS, nullptr, 0);	// ピクセルシェーダーを設定
	for (int i = 0; i < m_pBuffers.size(); ++i)	// バッファの数だけ繰り返す
		pContext->PSSetConstantBuffers(i, 1, &m_pBuffers[i]);	// バッファを設定
	for (int i = 0; i < m_pTextures.size(); ++i)	// テクスチャの数だけ繰り返す
		pContext->PSSetShaderResources(i, 1, &m_pTextures[i]);	// テクスチャを設定
}
HRESULT PixelShader::MakeShader(void* pData, UINT size)
{
	return GetDevice()->CreatePixelShader(pData, size, NULL, &m_pPS);	// ピクセルシェーダーを作成
}