﻿#include "BackShadow.h"
#include "Sprite.h"
#include "Geometory.h"

#define RTV_3D_SIZE_WIDTH	(1280.0f / 1.5f)	//3D空間上のレンダーの表示サイズX
#define RTV_3D_SIZE_HEIGHT	(-720.0f / 1.5f)	//3D空間上のレンダーの表示サイズY
#define RTV_3D_POS_WIDTH	(640.0f)			//3D空間上のレンダー表示の原点X
#define RTV_3D_POS_HEIGHT	(360.0f)			//3D空間上のレンダー表示の原点Y

int testw = 10;
int testh = 10;

BackShadow::BackShadow()
	: m_pRTVTexture(nullptr)
	, m_pRTV_BS(nullptr)
	, m_pDSV_BS(nullptr)
	, m_pCamera(nullptr)
	, m_pShadowPlayer(nullptr)
	, m_indexX(0)
	, m_indexY(0)
	, m_castPosX(0)
	, m_castPosY(0)
	, m_SPposX(0.0f)
	, m_SPposY(0.0f)
	, m_SPpos(0.0f, 0.0f, 0.0f)
	, m_alpha{0}
	, m_underAlpha{0}
	, m_Player_a{0}
	, m_sumAlpha(0)
	, m_alphaData(0)
	, m_noAlphaData(0)
	, m_collisionFlag(false)
	, m_upFlag(false)
	, m_LRcheck(false)
{
	//レンダー表示関連の確保
	m_pRTVTexture = new Texture;
	m_pRTV_BS = new RenderTarget;
	m_pDSV_BS = new DepthStencil;

	//レンダーターゲット制作
	m_pRTV_BS->Create(DXGI_FORMAT_R8G8B8A8_UNORM, 1280 / 2, 720 / 2);//レンダー内の大きさ
	m_pDSV_BS->Create(m_pRTV_BS->GetWidth(), m_pRTV_BS->GetHeight(), true);
	SetRenderTargets(1, &m_pRTV_BS, m_pDSV_BS);

	//影のみに表示するものの確保
	m_pShadowPlayer = new ShadowP;

	//コピー用フォーマット作成
	m_pRTV_BS->CreateReadBuffer();

	//影の当たり判定用変数初期化
	m_SPpos = m_pShadowPlayer->NowPos();

	//進行方向初期化
	m_LRcheck = m_pShadowPlayer->isUse();
}

BackShadow::~BackShadow()
{
	if (m_pRTVTexture)
	{
		delete m_pRTVTexture;
		m_pRTVTexture = nullptr;
	}
	if (m_pRTV_BS)
	{
		delete m_pRTV_BS;
		m_pRTV_BS = nullptr;
	}
	if (m_pDSV_BS)
	{
		delete m_pDSV_BS;
		m_pDSV_BS = nullptr;
	}
	if (m_pShadowPlayer)
	{
		delete m_pShadowPlayer;
		m_pShadowPlayer = nullptr;
	}
}

/**
 * @fn
 * 影(レンダー)の更新
 * 影プレイヤーの更新
 */
void BackShadow::Update()
{
	m_pShadowPlayer->Update();
}

/**
 * @fn
 * 基本動作　＝　影(レンダー)の描画
 * 基本以外　＝　影のテクスチャ情報を取得、当たり判定用変数の更新
 * @brief 影(レンダー)の描画
 * @param (Player* Player) プレイヤーのポインタ
 * @param (ObjectMng* Obj) オブジェのポインタ
 * @sa 参照すべき関数を書けばリンクが貼れる
 * @detail 3D空間にあるものを表示したいときは「引数を増やす」こと
 */
void BackShadow::Draw(ObjectCamera* m_pobjcamera, ObjectMng* Obj, Coin* Coin1, Coin* Coin2, Coin* Coin3)
{
	//深度バッファのクリア
	//m_pDSV_BS->Clear();

	//レンダーの色設定し、その色で画面クリア
	float color[4] = { 1.0f, 0.5f, 1.0f, 0.1f };
	m_pRTV_BS->Clear(color);
	//m_pRTV_BS->Clear();

	SetRenderTargets(1, &m_pRTV_BS, m_pDSV_BS);//レンダーの設定

	//ここに表示したいものを持ってくる
	DirectX::XMFLOAT4X4 viewMatrix = m_pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 projectionMatrix = m_pCamera->GetProjectionMatrix();
	Obj->Draw(viewMatrix, projectionMatrix);
	m_pShadowPlayer->Draw(viewMatrix, projectionMatrix);


	RenderTarget* pRTV;
	pRTV = GetDefaultRTV();

	SetRenderTargets(1, &pRTV, nullptr);

	//コインをフィールド上に表示
	if (Coin1->IsFirstCollected == false)
	{
		Coin1->Draw(270.0f, 355.0f, 0.0f, 20.0f, 20.0f, 1);	//左 y=120.0f
	}
	if (Coin2->IsFirstCollected == false)
	{
		Coin2->Draw(500.0f, 320.0f, 0.0f, 20.0f, 20.0f, 2);	//真ん中
	}
	if (Coin3->IsFirstCollected == false)
	{
		Coin3->Draw(1200.0f, 300.0f, 0.0f, 20.0f, 20.0f, 3);	//右
	}

	//レンダーターゲットの色情報読み取り
	m_pRTV_BS->Read([&](const void* colorData, UINT width, UINT height) {
		struct Color {
			BYTE r, g, b, a;
		};

		//レンダー当たり判定用変数更新
		m_SPpos = m_pShadowPlayer->NowPos();				//影の座標を所得
		m_SPposX = ((m_SPpos.x - 5.0f) / 10.0f) * (-1);		//X軸をレンダーのウィンドウ座標に合わせて変換
		m_SPposY = ((m_SPpos.y - 3.0f) / 6.0f) * (-1);		//Y軸をレンダーのウィンドウ座標に合わせて変換

		//コイン
		if (Coin1->IsFirstCollected == false)
		{
			m_1Cpos = Coin1->GetPosition();
		}

		m_Csize = Coin1->GetSize();

		if (Coin2->IsFirstCollected == false)
		{
			m_2Cpos = Coin2->GetPosition();
		}

		m_Csize = Coin2->GetSize();

		if (Coin3->IsFirstCollected == false)
		{
			m_3Cpos = Coin3->GetPosition();
		}

		m_Csize = Coin3->GetSize();


		m_cast1CposX = static_cast<int>(m_1Cpos.x / 2.0f);
		m_cast1CposY = static_cast<int>(m_1Cpos.y / 2.0f);

		m_cast2CposX = static_cast<int>(m_2Cpos.x / 2.0f);
		m_cast2CposY = static_cast<int>(m_2Cpos.y / 2.0f);

		m_cast3CposX = static_cast<int>(m_3Cpos.x / 2.0f);
		m_cast3CposY = static_cast<int>(m_3Cpos.y / 2.0f);

		m_castCsizeX = static_cast<int>(m_Csize.x / 2.0f);
		m_castCsizeY = static_cast<int>(m_Csize.y / 2.0f);

		//キャスト用
		m_castPosX = static_cast<int>(m_SPposX * width);
		m_castPosY = static_cast<int>(m_SPposY * height);

		//プレイヤーの位置とスクリーン左上の座標の差分を、スクリーンの横幅で割ると配列のインデックスが求められる
		//影のプレイヤーはレンダーにのみ表示している->レンダー幅が影プレイヤーのウィンドウ幅
		//ただし、今回のプレイヤーの位置とスクリーン左上の座標の差分はないためそのままで、かつ、元の値を0～1にしているため掛け算を使用
		//(例)(int)((m_pos.x - screenPos.x) / screenWidth)
		m_indexX = m_castPosX;
		m_indexY = m_castPosY;

		//コイン
		CoinCollection(Coin1, Coin2, Coin3);

		const Color* pData = reinterpret_cast<const Color*>(colorData);
		//m_Player_a = pData[m_indexY * width + m_indexX].a;	//プレイヤーの位置のα値を見たい
		//m_alpha[0] = pData[m_indexY * width + m_indexX + 11].a;

		//if (m_alpha[0] > 128)
		//{
		//	m_pShadowPlayer->Use();//エラー237-392
		//}

		//進行方向チェック
		m_LRcheck = m_pShadowPlayer->isUse();

		for (int h = 0; h < height; ++h)
		{
			for (int w = 0; w < width; ++w)
			{
				m_alphaData = 1;
				m_noAlphaData = 1;
				m_sumAlpha = 0;
				for (int j = 0; j < 5; j++)
				{
					for (int i = 0; i < 5; i++)
					{
						if (m_LRcheck == false)
						{
							m_alpha = pData[(m_indexY - j) * width + m_indexX + 10 + i].a;	//レンダーウィンドウのα値を左上から一つずつ見てる
						}
						else
						{
							m_alpha = pData[(m_indexY - j) * width + m_indexX - 10 - i].a;
						}

						if (m_alpha > 200)
						{
							m_sumAlpha += m_alpha;
							m_alphaData++;
						}
						if (m_alpha < 100)
						{
							m_noAlphaData++;
						}
					}
				}
				
				if (ShadowCollision(m_sumAlpha, m_alphaData, m_noAlphaData) || ShadowEdgeCollision(h, width))
				{
					m_collisionFlag = true;
					break;
				}
			}
			if (m_collisionFlag)
			{
				m_collisionFlag = false;
				break;
			}
		}
		m_underAlpha = pData[(m_indexY + 2) * width + m_indexX + 10].a;
		ShadowUnderCollision(m_underAlpha);
	});

	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみ考慮
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(RTV_3D_POS_WIDTH, RTV_3D_POS_HEIGHT, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	mat[1] = m_pCamera->GetShadowViewMatrix();
	mat[2] = m_pCamera->GetShadowProjectionMatrix();

	//スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(RTV_3D_SIZE_WIDTH, RTV_3D_SIZE_HEIGHT));
	Sprite::SetTexture(m_pRTV_BS);
	Sprite::Draw();
}

/**
 * @fn
 * レンダー用カメラのセット
 */
void BackShadow::SetShadowCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

/**
 * @fn
 * α値を使った当たり判定
 * @brief 影(レンダー)の当たり判定
 * @param (BYTE alpha)	レンダー画面のα値(一ピクセル分)
 * @param (int h)　		レンダー画面の高さの参照値(一ピクセルごと)
 * @param (int w)　		レンダー画面の幅の参照値(一ピクセルごと)
 * @param (UINT width)	レンダー画面の幅
 * @return true or false(何もないならfalseで返す)
 * @detail 今はまだ描画があるか見てるだけ
 */
bool BackShadow::ShadowCollision(int sumAlpha, int cntAlpha, int noAlpha)
{
	//変数確認プログラム
	//if (cntAlpha > 13 && noAlpha > 10)
	//{
	//	int a = sumAlpha / (cntAlpha + noAlpha);
	//	int b = 0;
	//}

	// 左右のα値の参照
	if (sumAlpha / cntAlpha > 240 && cntAlpha > 25)
	{// 壁
		m_pShadowPlayer->Use();
		return true;
	}
	if (sumAlpha / (cntAlpha + noAlpha) > 128 && cntAlpha > 13 && noAlpha > 10)
	{// 階段
		m_pShadowPlayer->Jump();
		return true;
	}

	return false;
}

bool BackShadow::ShadowUnderCollision(BYTE underAlpha)
{
	// 足元のα値参照
	if (underAlpha > 240)
	{
		m_pShadowPlayer->SetFooting(true);
		return true;
	}
	else
	{
		m_pShadowPlayer->SetFooting(false);
	}
	return false;
}

bool BackShadow::ShadowEdgeCollision(int h, UINT width)
{
	if (h * width + 2 == m_indexY * width + m_indexX)
	{// 左の画面端
		m_pShadowPlayer->Use();
		return true;
	}
	if (h * width + width - 1 == m_indexY * width + m_indexX)
	{// 右の画面端
		m_pShadowPlayer->Use();
		return true;
	}
	return false;
}

//コインの当たり判定＆処理
void BackShadow::CoinCollection(Coin* Coin1, Coin* Coin2, Coin* Coin3)
{
	// コインの取得処理
	// 影の座標
	int shadowPosX = m_castPosX;
	int shadowPosY = m_castPosY;


	if (m_cast1CposX + m_castCsizeX / 2 > shadowPosX &&
		m_cast1CposX - m_castCsizeX / 2 < shadowPosX &&
		m_cast1CposY + m_castCsizeY / 2 > shadowPosY &&
		m_cast1CposY - m_castCsizeY / 2 < shadowPosY)
	{
		// 影とコインが重なったらコインを取得する
		Coin1->SetCollect(true);
	}

	if (m_cast2CposX + m_castCsizeX / 2 > shadowPosX &&
		m_cast2CposX - m_castCsizeX / 2 < shadowPosX &&
		m_cast2CposY + m_castCsizeY / 2 > shadowPosY &&
		m_cast2CposY - m_castCsizeY / 2 < shadowPosY)
	{
		// 影とコインが重なったらコインを取得する
		Coin2->SetCollect(true);
	}

	if (m_cast3CposX + m_castCsizeX / 2 > shadowPosX &&
		m_cast3CposX - m_castCsizeX / 2 < shadowPosX &&
		m_cast3CposY + m_castCsizeY / 2 > shadowPosY &&
		m_cast3CposY - m_castCsizeY / 2 < shadowPosY)
	{
		// 影とコインが重なったらコインを取得する
		Coin3->SetCollect(true);
	}
}
