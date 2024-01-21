#include "BackShadow.h"
#include "Sprite.h"
#include "Geometory.h"

#define RTV_3D_SIZE_WIDTH	(1280.0f / 1.5f)	//3D空間上のレンダーの表示サイズX
#define RTV_3D_SIZE_HEIGHT	(-800.0f / 1.5f)	//3D空間上のレンダーの表示サイズY
#define RTV_3D_POS_WIDTH	(640.0f)			//3D空間上のレンダー表示の原点X
#define RTV_3D_POS_HEIGHT	(330.0f)			//3D空間上のレンダー表示の原点Y

int testw = 10;
int testh = 10;

BackShadow::BackShadow()
	: m_pRTVTexture(nullptr)
	, m_pRTV_BS(nullptr)
	, m_pDSV_BS(nullptr)
	, m_pCamera(nullptr)
	, m_pShadowPlayer(nullptr)
	,m_pScreen(nullptr)//27
	, m_indexX(0)
	, m_indexY(0)
	, m_castPosX(0)
	, m_castPosY(0)
	, m_SPposX(0.0f)
	, m_SPposY(0.0f)
	, m_SPpos(0.0f, 0.0f, 0.0f)
	, m_alpha{0}
	, m_alpha2{0}
	, m_underAlpha{0}
	, m_underAlpha2{0}
	, m_PleyerSenter{0}
	, m_Player_a{0}
	, m_sumAlpha(0)
	, m_alphaData(0)
	, m_noAlphaData(0)
	, m_1Cpos(0.0f, 0.0f, 0.0f)		//コイン1の座標
	, m_cast1CposX(0)				//コイン1のX変換座標用
	, m_cast1CposY(0)				//コイン1のY変換座標用
	, m_2Cpos(0.0f, 0.0f, 0.0f)		//コイン2の座標
	, m_cast2CposX(0)		   		//コイン2のX変換座標用
	, m_cast2CposY(0)		   		//コイン2のY変換座標用
	, m_3Cpos(0.0f, 0.0f, 0.0f)		//コイン3の座標
	, m_cast3CposX(0)				//コイン3のX変換座標用
	, m_cast3CposY(0)				//コイン3のY変換座標用
	, m_castCsizeX(0)				//コインのXサイズ
	, m_castCsizeY(0)				//コインのYサイズ
	, m_Goalpos(0.0f,0.0f,0.0f)		//ゴールの座標
	, m_castGoalposX(0)
	, m_castGoalposY(0)
	, m_castGoalsizeX(0)			//ゴールのXサイズ
	, m_castGoalsizeY(0)			//ゴールのYサイズ
	, m_collisionFlag(false)
	, m_upFlag(false)
	, m_LRcheck(false)
	,m_pSDSESdCoin(nullptr)
	,m_pSVSESdCoin(nullptr)

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
	m_pShadowPlayer = new ShadowP();
	m_pScreen = new Screen();//27

	//コピー用フォーマット作成
	m_pRTV_BS->CreateReadBuffer();

	//影の当たり判定用変数初期化
	m_SPpos = m_pShadowPlayer->NowPos();

	//進行方向初期化
	m_LRcheck = m_pShadowPlayer->isUse();

	m_pSDSESdCoin = LoadSound("Assets/Sound/SE/Coinkaisyuu_Oobayashi.wav");

	// PixelShader
	m_pPS[0] = new PixelShader;
	m_pPS[1] = new PixelShader;
	m_pPS[2] = new PixelShader;
	m_pPS[0]->Load("Assets/Shader/PS_Shadow.cso");
	m_pPS[1]->Load("Assets/Shader/PS_Binarization.cso");
	m_pPS[2]->Load("Assets/Shader/PS_Sprite.cso");
}

BackShadow::~BackShadow()
{
	for (int i = 0; i < 2; i++)
	{
		if (m_pPS[i])
		{
			delete m_pPS[i];
			m_pPS[i] = nullptr;
		}
	}
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
void BackShadow::Update(float tick)
{

	m_pShadowPlayer->Update(tick);

	
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
void BackShadow::Draw(ObjectCamera* m_pobjcamera, ObjectMng* Obj, Coin* Coin1, Coin* Coin2, Coin* Coin3, Goal* Goal)
{
	//深度バッファのクリア
	//m_pDSV_BS->Clear();

	//レンダーの色設定し、その色で画面クリア
	float color[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	m_pRTV_BS->Clear(color);
	//m_pRTV_BS->Clear();

	SetRenderTargets(1, &m_pRTV_BS, m_pDSV_BS);//レンダーの設定

	//ここに表示したいものを持ってくる

	DirectX::XMFLOAT4X4 viewMatrix = m_pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 projectionMatrix = m_pCamera->GetProjectionMatrix();
    Obj->Draw(viewMatrix, projectionMatrix,false);

	

	m_pShadowPlayer->Draw(viewMatrix, projectionMatrix);


	//ゴールをフィールド上に表示
	if (Goal->IsGoal == false)
	{
		Goal->Draw(1230.0f, 70.0f, 0.0f, 60.0f, 60.0f);
	}

	//コインをフィールド上に表示
	//Drawの１,２個目の数値をいじればコイン描画座標が変わる
	if (Coin1->IsCoinCollected == false)
	{
		Coin1->Draw(270.0f, 0.0f, 0.0f,40.0f, 40.0f, 1);	//左 y=120.0f
	}
	if (Coin2->IsCoinCollected == false)
	{
		Coin2->Draw(500.0f, 320.0f, 0.0f, 40.0f, 40.0f, 2);	//真ん中
	}
	if (Coin3->IsCoinCollected == false)
	{
		Coin3->Draw(1200.0f, 300.0f, 0.0f, 40.0f, 40.0f, 3);	//右
	}

	RenderTarget* pRTV;
	pRTV = GetDefaultRTV();
	SetRenderTargets(1, &pRTV, nullptr); 

	//レンダーターゲットの色情報読み取り
	m_pRTV_BS->Read([&](const void* colorData, UINT width, UINT height) {
		struct Color {
			BYTE r, g, b, a;
		};

		//レンダー当たり判定用変数更新
		m_SPpos = m_pShadowPlayer->NowPos();				//影の座標を所得
		m_SPposX = ((m_SPpos.x - 5.0f) / 10.0f) * (-1);		//X軸をレンダーのウィンドウ座標に合わせて変換
		m_SPposY = ((m_SPpos.y - 3.0f) / 6.0f) * (-1);		//Y軸をレンダーのウィンドウ座標に合わせて変換

		//キャスト用
		m_castPosX = static_cast<int>(m_SPposX * width);
		m_castPosY = static_cast<int>(m_SPposY * height);

		//ゴール
		if (Goal->IsGoal == false)
		{
			m_Goalpos = Goal->GetPosition();
		}
		m_Goalsize = Goal->GetSize();

		//コイン
		if (Coin1->IsCoinCollected == false)
		{
			m_1Cpos = Coin1->GetPosition();
		}
		m_Csize = Coin1->GetSize();

		if (Coin2->IsCoinCollected == false)
		{
			m_2Cpos = Coin2->GetPosition();
		}
		m_Csize = Coin2->GetSize();

		if (Coin3->IsCoinCollected == false)
		{
			m_3Cpos = Coin3->GetPosition();
		}
		m_Csize = Coin3->GetSize();

		//ゴールの座標変換
		m_castGoalposX = static_cast<int>(m_Goalpos.x / 2.0f);
		m_castGoalposY = static_cast<int>(m_Goalpos.y / 2.0f);
		//ゴールのサイズ変換
		m_castGoalsizeX = static_cast<int>(m_Goalsize.x / 2.0f);
		m_castGoalsizeY = static_cast<int>(m_Goalsize.y / 2.0f);

		//コインの座標変換
		m_cast1CposX = static_cast<int>(m_1Cpos.x / 2.0f);
		m_cast1CposY = static_cast<int>(m_1Cpos.y / 2.0f);

		m_cast2CposX = static_cast<int>(m_2Cpos.x / 2.0f);
		m_cast2CposY = static_cast<int>(m_2Cpos.y / 2.0f);

		m_cast3CposX = static_cast<int>(m_3Cpos.x / 2.0f);
		m_cast3CposY = static_cast<int>(m_3Cpos.y / 2.0f);
		//コインのサイズ変換
		m_castCsizeX = static_cast<int>(m_Csize.x / 2.0f);
		m_castCsizeY = static_cast<int>(m_Csize.y / 2.0f);

		//プレイヤーの位置とスクリーン左上の座標の差分を、スクリーンの横幅で割ると配列のインデックスが求められる
		//影のプレイヤーはレンダーにのみ表示している->レンダー幅が影プレイヤーのウィンドウ幅
		//ただし、今回のプレイヤーの位置とスクリーン左上の座標の差分はないためそのままで、かつ、元の値を0～1にしているため掛け算を使用
		//(例)(int)((m_pos.x - screenPos.x) / screenWidth)
		m_indexX = m_castPosX;
		m_indexY = m_castPosY;

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
							m_alpha = pData[(m_indexY - j) * width + m_indexX - 10 - i].a;	//(プレイヤーposY - 高さ) * 横幅 + プレイヤーposX - サイズ - 見たい横幅
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
				if (m_LRcheck == false)
				{
					m_alpha = pData[(m_indexY - 50) * width + m_indexX + 10].a;	//レンダーウィンドウのα値を左上から一つずつ見てる
					m_alpha2 = pData[(m_indexY - 10) * width + m_indexX + 10].a;	//レンダーウィンドウのα値を左上から一つずつ見てる
					//コイン
					CoinCollection(Coin1, Coin2, Coin3, m_alpha, m_alpha2);
				}
				else
				{
					m_alpha = pData[(m_indexY - 50) * width + m_indexX - 10].a;	//レンダーウィンドウのα値を左上から一つずつ見てる
					m_alpha2 = pData[(m_indexY - 10) * width + m_indexX - 10].a;
					//コイン
					CoinCollection(Coin1, Coin2, Coin3, m_alpha, m_alpha2);
				}
			}
			if (m_collisionFlag)
			{
				m_collisionFlag = false;
				break;
			}
		}
		m_underAlpha	= pData[(m_indexY + 2) * width + m_indexX + 10].a;
		m_underAlpha2	= pData[(m_indexY - 2) * width + m_indexX + 10].a;
		ShadowUnderCollision(m_underAlpha, m_underAlpha2);
	});



	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみ考慮
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(RTV_3D_POS_WIDTH, RTV_3D_POS_HEIGHT, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	mat[1] = m_pCamera->GetShadowViewMatrix();
	mat[2] = m_pCamera->GetShadowProjectionMatrix();

	SetSamplerState(SAMPLER_POINT);
	//スプライトの設定
	Sprite::SetPixelShader(m_pPS[1]);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(RTV_3D_SIZE_WIDTH, RTV_3D_SIZE_HEIGHT));
	Sprite::SetTexture(m_pRTV_BS);
	Sprite::Draw();
	

	//m_pPS[0]->Bind();
	//m_pPS[0]->WriteBuffer(0, mat);

	Sprite::SetPixelShader(m_pPS[2]);
	SetSamplerState(SAMPLER_LINEAR);
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
	if (sumAlpha / cntAlpha > 200 && cntAlpha > 25)
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

bool BackShadow::ShadowUnderCollision(BYTE underAlpha, BYTE underAlpha2)
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
	if (underAlpha2 > 240)
	{
		m_pShadowPlayer->ShadowPupY();
		return true;
	}
	return false;
}

bool BackShadow::ShadowEdgeCollision(int h, UINT width)
{
	if (h * width + 15== m_indexY * width + m_indexX)
	{// 左の画面端
		m_pShadowPlayer->Use();
		return true;
	}
	if (h * width + width - 4 == m_indexY * width + m_indexX)
	{// 右の画面端
		m_pShadowPlayer->Use();
		return true;
	}
	return false;
}

//コインの当たり判定＆処理
void BackShadow::CoinCollection(Coin* Coin1, Coin* Coin2, Coin* Coin3, BYTE RegAlpha, BYTE BodyAlpha)
{
	// コインの取得処理
	// 影の座標
	int shadowPosX = m_castPosX;
	int shadowPosY = m_castPosY;
	int shadowSizeX = 10;
	int shadowSizeY = 50;

	if (BodyAlpha > 240)
	{
		int a = 0;
	}

	if (m_cast1CposX + m_castCsizeX / 2 > shadowPosX + shadowSizeX &&		// コインの右側
		m_cast1CposX - m_castCsizeX / 2 < shadowPosX - shadowSizeX &&		// コインの左側
		m_cast1CposY + m_castCsizeY / 2 > shadowPosY &&		// コインの下側
		m_cast1CposY - m_castCsizeY / 2 < shadowPosY - shadowSizeY)		// コインの上側
	{
		// 影とコインが重なったらコインを取得する
		if (RegAlpha > 240 || BodyAlpha > 240)
		{
			Coin1->SetCollect(true);
			m_pSVSESdCoin = PlaySound(m_pSDSESdCoin);
		}
	}

	if (m_cast2CposX + m_castCsizeX / 2 > shadowPosX + shadowSizeX &&
		m_cast2CposX - m_castCsizeX / 2 < shadowPosX - shadowSizeX &&
		m_cast2CposY + m_castCsizeY / 2 > shadowPosY &&
		m_cast2CposY - m_castCsizeY / 2 < shadowPosY - shadowSizeY)
	{
		// 影とコインが重なったらコインを取得する
		if (RegAlpha > 240 || BodyAlpha > 240)
		{
			Coin2->SetCollect(true);
			m_pSVSESdCoin = PlaySound(m_pSDSESdCoin);
		}
	}

	if (m_cast3CposX + m_castCsizeX / 2 > shadowPosX + shadowSizeX &&
		m_cast3CposX - m_castCsizeX / 2 < shadowPosX - shadowSizeX &&
		m_cast3CposY + m_castCsizeY / 2 > shadowPosY &&
		m_cast3CposY - m_castCsizeY / 2 < shadowPosY - shadowSizeY)
	{
		// 影とコインが重なったらコインを取得する
		if (RegAlpha > 240 || BodyAlpha > 240)
		{
			Coin3->SetCollect(true);
			m_pSVSESdCoin = PlaySound(m_pSDSESdCoin);
		}
	}

	if (RegAlpha > 240)
	{
		int a = 0;
	}
}

//ゴールの当たり判定＆処理
void BackShadow::GoalCollision(Goal* Goal)
{
	// 影の座標
	int shadowPosX = m_castPosX;
	int shadowPosY = m_castPosY;

	if (m_castGoalposX + m_castGoalsizeX / 2 > shadowPosX &&
		m_castGoalposX - m_castGoalsizeX / 2 < shadowPosX &&
		m_castGoalposY + m_castGoalsizeY / 2 > shadowPosY &&
		m_castGoalposY - m_castGoalsizeY / 2 < shadowPosY)
	{
		// 影とダイヤ型が重なったらゴールする
		Goal->SetGoal(true);
	}
}
