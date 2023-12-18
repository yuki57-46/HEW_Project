#include "Texture.h"
#include "DirectXTex/TextureLoad.h"

/// <summary>
/// テクスチャ
/// </summary>
Texture::Texture()
	: m_width(0), m_height(0)
	, m_pTex(nullptr)
	, m_pSRV(nullptr)
{
}
Texture::~Texture()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pTex);
}
HRESULT Texture::Create(const char* fileName)
{
	HRESULT hr = S_OK;

	// 文字変換
	wchar_t wPath[MAX_PATH];
	size_t wLen = 0;
	MultiByteToWideChar(0, 0, fileName, -1, wPath, MAX_PATH);  
	// ↑ ワイド文字に変換:引数(文字コード, フラグ, 変換元文字列, 文字数, 変換先文字列, 文字数)
	//	 文字コード: 0 = UTF-16 ?
	//	 フラグ: 0 = デフォルト
	//	 変換元文字列: fileName
	//	 文字数: -1 = 文字列の終端まで
	//	 変換先文字列: wPath
	//	 文字数: MAX_PATH = 最大文字数(パスが読み込める最大文字数)

	// ファイル別読み込み
	DirectX::TexMetadata mdata;	// メタデータ
	DirectX::ScratchImage image;	// 画像データ
	if (strstr(fileName, ".tga"))
		hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);	// TGAファイルが読み込まれたらhrにS_OKが入る
	else
		hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
		// ↑ WIC(Windows Imaging Component)を使用して画像を読み込む
		//	 引数(ファイルパス, フラグ, メタデータ, 画像データ)
		//	 ファイルパス: wPath
		//	 フラグ: WIC_FLAGS_NONE = デフォルト
		//	 メタデータ: mdata
		//	 画像データ: image
	if (FAILED(hr)) {
		return E_FAIL;	// 読み込み失敗
	}

	// シェーダリソース生成
	hr = CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), mdata, &m_pSRV);
	// ↑ シェーダリソースビューを作成
	//	 引数(デバイス, 画像データ, 画像数, メタデータ, シェーダリソースビュー)
	//	 デバイス: GetDevice()
	//	 画像データ: image.GetImages() = 画像データのポインタを取得
	//	 画像数: image.GetImageCount() = 画像データの数を取得
	//	 メタデータ: mdata
	//	 シェーダリソースビュー: m_pSRV
	if (SUCCEEDED(hr))
	{
		m_width = (UINT)mdata.width;	// メタデータから幅を取得
		m_height = (UINT)mdata.height;	// メタデータから高さを取得
	}
	return hr;
}
HRESULT Texture::Create(DXGI_FORMAT format, UINT width, UINT height, const void* pData)
// ↑ 引数(フォーマット, 幅, 高さ, データ) DXGI_FORMAT = テクスチャのフォーマット, UINT = unsigned int,幅と高さ, データ
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height); 
	// ↑ テクスチャの設定を作成
	//	 引数(フォーマット, 幅, 高さ)
	return CreateResource(desc, pData);	// テクスチャリソースを作成
}

UINT Texture::GetWidth() const
{
	return m_width;
}
UINT Texture::GetHeight() const
{
	return m_height;
}
ID3D11ShaderResourceView* Texture::GetResource() const
{
	return m_pSRV;
}

D3D11_TEXTURE2D_DESC Texture::MakeTexDesc(DXGI_FORMAT format, UINT width, UINT height)
// ↑ テクスチャの設定を作成 引数(フォーマット, 幅, 高さ)
{
	D3D11_TEXTURE2D_DESC desc = {};	// テクスチャの設定
	desc.Usage = D3D11_USAGE_DEFAULT;	// 使用方法: D3D11_USAGE_DEFAULT = GPUが読み書き可能
	desc.Format = format;			// フォーマット
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// バインドフラグ: D3D11_BIND_SHADER_RESOURCE = シェーダリソースとしてバインド
	desc.Width = width;			// 幅
	desc.Height = height;		// 高さ
	desc.MipLevels = 1;		// ミップマップレベル
	desc.ArraySize = 1;	// 配列サイズ
	desc.SampleDesc.Count = 1;	// サンプル数
	return desc;	// テクスチャの設定を返す
}
HRESULT Texture::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
// ↑ テクスチャリソースを作成 引数(テクスチャの設定, データ)
{
	HRESULT hr = E_FAIL;	// 戻り値 初期値: E_FAIL = 失敗

	// テクスチャ作成
	D3D11_SUBRESOURCE_DATA data = {};	// サブリソースデータ
	data.pSysMem = pData;	// システムメモリにデータを設定
	data.SysMemPitch = desc.Width * 4;	// ピッチ: 幅 * 4
	hr = GetDevice()->CreateTexture2D(&desc, pData ? &data : nullptr, &m_pTex);
	// ↑ 2Dテクスチャを作成
	//	 引数(テクスチャの設定, 初期データ, 2Dテクスチャ)
	//	 pData ? &data : pDataがあればdataのポインタを参照,無い(=NULL)の場合は初期データは無し、後に更新される?
	if (FAILED(hr)) { return hr; }

	// 設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};	// シェーダリソースビューの設定
	switch (desc.Format)
	{
	default:						srvDesc.Format = desc.Format;			break;	// フォーマット
	case DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;
	// ↑ DXGI_FORMAT_R32_TYPELESS = 32ビットの型なしデータ, DXGI_FORMAT_R32_FLOAT = 32ビットの浮動小数点データ
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	// ビューの次元: D3D11_SRV_DIMENSION_TEXTURE2D = 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;	// ミップマップレベル
	// 生成
	hr = GetDevice()->CreateShaderResourceView(m_pTex, &srvDesc, &m_pSRV);	// シェーダリソースビューを作成
	if (SUCCEEDED(hr))
	{
		m_width = desc.Width;
		m_height = desc.Height;
	}
	return hr;
}

/// <summary>
/// レンダーターゲット
/// </summary>
RenderTarget::RenderTarget()
	: m_pRTV(nullptr)
{
}
RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_pRTV);
}
void RenderTarget::Clear()
{
	static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Clear(color);
}
void RenderTarget::Clear(const float* color)
{
	GetContext()->ClearRenderTargetView(m_pRTV, color);	// レンダーターゲットビューをクリア
}
HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);		// テクスチャの設定を作成
	desc.BindFlags |= D3D11_BIND_RENDER_TARGET;	// バインドフラグにレンダーターゲットを追加
	return CreateResource(desc);	// テクスチャリソースを作成し、戻り値を返す
}
HRESULT RenderTarget::CreateFromScreen()
{
	HRESULT hr;

	// バックバッファのポインタを取得
	ID3D11Texture2D* pBackBuffer = NULL;	// バックバッファ
	hr = GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);	// バックバッファを取得
	if (FAILED(hr)) { return hr; }

	// バックバッファへのポインタを指定してレンダーターゲットビューを作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};	// レンダーターゲットビューの設定
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;	// ビューの次元: D3D11_RTV_DIMENSION_TEXTURE2D = 2Dテクスチャ
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// ↑ DXGI_FORMAT_R8G8B8A8_UNORM_SRGB = 32ビットのRGBA各8ビットの符号なし整数データ
	rtvDesc.Texture2D.MipSlice = 0;	// ミップマップレベル
	hr = GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);
	// ↑ レンダーターゲットビューを作成
	//	 引数(テクスチャ, レンダーターゲットビューの設定, レンダーターゲットビュー)
	if (SUCCEEDED(hr))
	{
		D3D11_TEXTURE2D_DESC desc;	// テクスチャの設定
		m_pTex->GetDesc(&desc);	// テクスチャの設定を取得
		m_width = desc.Width;	// 幅
		m_height = desc.Height;	// 高さ
	}
	return hr;
}
ID3D11RenderTargetView* RenderTarget::GetView() const
{
	return m_pRTV;
}
HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// テクスチャリソース作成
	HRESULT hr = Texture::CreateResource(desc, nullptr);	// テクスチャリソースを作成
	if (FAILED(hr)) { return hr; }

	// 設定
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};	// レンダーターゲットビューの設定
	rtvDesc.Format = desc.Format;	// フォーマット
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// ↑ ビューの次元: D3D11_RTV_DIMENSION_TEXTURE2D = 2Dテクスチャ

	// 生成
	return GetDevice()->CreateRenderTargetView(m_pTex, &rtvDesc, &m_pRTV);	// レンダーターゲットビューを作成
}

/// <summary>
/// 深度テクスチャ
/// </summary>
DepthStencil::DepthStencil()
	: m_pDSV(nullptr)
{
}
DepthStencil::~DepthStencil()
{
	SAFE_RELEASE(m_pDSV);
}
void DepthStencil::Clear()
{
	GetContext()->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
{
	// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
	// ↑msdnの合成の項目を参照
	D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);
	// ↑ テクスチャの設定を作成
	//	 引数(フォーマット, 幅, 高さ)
	//	 フォーマット: useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS
	//	 ↑ useStencilが真ならDXGI_FORMAT_R24G8_TYPELESSを、偽ならDXGI_FORMAT_R32_TYPELESSを返す。
	//   DXGI_FORMAT_R24G8_TYPELESS = 24ビットの深度と8ビットのステンシルの情報を持つフォーマット
	//   DXGI_FORMAT_R32_TYPELESS = 32ビットの深度情報を持つフォーマット
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;	// バインドフラグに深度ステンシルを追加
	return CreateResource(desc);
}
ID3D11DepthStencilView* DepthStencil::GetView() const
{
	return m_pDSV;
}
HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
{
	// ステンシル使用判定
	bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);
	// ↑ フォーマットがDXGI_FORMAT_R24G8_TYPELESSならuseStencilは真、そうでなければ偽

	// リソース生成
	desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;	// バインドフラグに深度ステンシルを追加
	HRESULT hr = Texture::CreateResource(desc, nullptr);	// テクスチャリソースを作成
	if (FAILED(hr)) { return hr; }

	// 設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};	// 深度ステンシルビューの設定
	dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	// ↑ フォーマット: useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT
	//   DXGI_FORMAT_D24_UNORM_S8_UINT = 24ビットの深度と8ビットのステンシルの情報を持つフォーマット
	//   DXGI_FORMAT_D32_FLOAT = 32ビットの浮動小数点の深度バッファを持つフォーマット
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	// ビューの次元: D3D11_DSV_DIMENSION_TEXTURE2D = 2Dテクスチャ

	// 生成
	return GetDevice()->CreateDepthStencilView(m_pTex, &dsvDesc, &m_pDSV);	// 深度ステンシルビューを作成し、戻り値を返す
}