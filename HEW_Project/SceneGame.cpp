#include "SceneGame.h"
#include "Geometory.h"
#include <DirectXMath.h>

SceneGame::SceneGame(SceneManager* pSceneManager)
	: m_pSound(nullptr)
	, m_pSourceVoice(nullptr)
	, m_pVS(nullptr)
	, m_pCamera{ nullptr, nullptr,nullptr }
	, m_pobjcamera(nullptr)
	, m_pRTV(nullptr)
	, m_pDSV(nullptr)
	, m_pUI(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pScreen(nullptr)
	, m_pSceneManager(pSceneManager)
{

	//RenderTarget* pRTV = GetDefaultRTV();  //デフォルトで使用しているRenderTargetViewの取得
	//DepthStencil* pDSV = GetDefaultDSV();  //デフォルトで使用しているDepthStencilViewの取得
	//SetRenderTargets(1, &pRTV, pDSV);      //DSVがnullだと2D表示になる

	//深度バッファ、レンダーターゲットの設定
	m_pRTV = GetDefaultRTV();	//デフォルトで使用しているRender Target Viewの取得
	m_pDSV = GetDefaultDSV();	//デフォルトで使用しているDepth Stencil Viewの取得
	SetRenderTargets(1, &m_pRTV, m_pDSV);		//DSVがnullだと２D表示になる

	//m_pPlayer = new Player();
	m_pVS = new VertexShader();

	m_pobjcamera = new ObjectCamera();

	//コイン系
	m_pUI = new ItemUI();
	m_pCoinCntUI = new CoinCntUI();
	m_pCoin = new Coin[3];

	//ゴール
	m_pGoal = new Goal();

	//カーテン
	m_pCurtainUI = new CurtainUI();

	//スクリーン
	m_pScreen = new Screen();

	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "ERROR", MB_OK);
	}

	m_pCamera[CAM_OBJ] = new CameraObject(m_pobjcamera);
	m_pCamera[CAM_DEBUG] = new CameraDebug();
	m_pCamera[CAM_SHADOW] = new CameraShadow();

	m_pBackShadow = new BackShadow;

	m_pObjectMng = new ObjectMng();
	//m_pDCamera = new CameraDebug();

	m_pBackShadow->SetShadowCamera(m_pCamera[CAM_SHADOW]);
	m_pSound = LoadSound("Assets/Sound/BGM/Ge-musi-nnA_Muto.wav", true); // サウンドファイルの読み込み

	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);
	m_pBackShadow->SetShadowCamera(m_pCamera[CAM_SHADOW]);
	m_pSourceVoice = PlaySound(m_pSound); // サウンドの再生
}

SceneGame::~SceneGame()
{
	if (m_pScreen)
	{
		delete[] m_pScreen;
		m_pScreen = nullptr;
	}
	if (m_pCurtainUI)
	{
		delete m_pCurtainUI;
		m_pCurtainUI = nullptr;
	}
	if (m_pGoal)
	{
		delete m_pGoal;
		m_pGoal = nullptr;
	}
	if (m_pCoin)
	{
		delete[] m_pCoin;
		m_pCoin = nullptr;
	}

	if (m_pCoinCntUI)
	{
		delete m_pCoinCntUI;
		m_pCoinCntUI = nullptr;
	}

	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}
	/*if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}*/
	if (m_pRTV)
	{
		delete m_pRTV;
		m_pRTV = nullptr;
	}
	if (m_pDSV)
	{
		delete m_pDSV;
		m_pDSV = nullptr;
	}
	if (m_pCamera)
	{
		for (int i = 0; i < MAX_CAMERA; i++)
		{
			delete m_pCamera[i];
			m_pCamera[i] = nullptr;
		}
	}
	if (m_pObjectMng)
	{
		delete m_pObjectMng;
		m_pObjectMng = nullptr;
	}
	if (m_pBackShadow)
	{
		delete m_pBackShadow;
		m_pBackShadow = nullptr;
	}
	if (m_pBackShadow)
	{
		delete m_pBackShadow;
		m_pBackShadow = nullptr;
	}
	m_pSourceVoice->Stop();
}

void SceneGame::Update(float tick)
{
	if (m_pobjcamera != nullptr)
		{
			m_pobjcamera->SetCamera(m_pCamera[CAM_SHADOW]);	// 例外スロー発生場所
		}

	m_pBackShadow->Update(tick);

	//m_pObjectMng->SetPlayer(m_pPlayer);
	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);

	m_pCamera[CAM_OBJ]->Update();

	//オブジェクト
	m_pobjcamera->SetCamera(m_pCamera[CAM_DEBUG]);
	m_pObjectMng->Update(tick);
	//m_pObject2D->Update();
	m_pCoinCntUI->Update();
	m_pCurtainUI->Update();

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f);
	DirectX::XMMATRIX mat = S * T;
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	Geometory::SetWorld(fMat);
}

void SceneGame::Draw()
{

	m_pDSV->Clear();
	SetRenderTargets(1, &m_pRTV, nullptr);

	static float rad = 0.0f;
	DirectX::XMFLOAT4X4 mat[3];

	m_pobjcamera->SetCamera(m_pCamera[CAM_SHADOW]);
	//背景

	m_pScreen->Draw(m_pCamera[CAM_OBJ]->GetViewMatrix(), m_pCamera[CAM_OBJ]->GetProjectionMatrix());
	m_pBackShadow->Draw(m_pobjcamera, m_pObjectMng, &m_pCoin[0], &m_pCoin[1], &m_pCoin[2], m_pGoal);

	//カーテン表示
	m_pCurtainUI->LeftDraw();
	m_pCurtainUI->RightDraw();

	//3D表示に変更
	SetRenderTargets(1, &m_pRTV, m_pDSV);

	//ワールド行列の計算
	DirectX::XMMATRIX world =
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	//ビュー行列の計算
	mat[1] = m_pCamera[CAM_OBJ]->GetViewMatrix();
	//プロジェクション行列の計算
	mat[2] = m_pCamera[CAM_OBJ]->GetProjectionMatrix();

	//行列をシェーダーへ設定
	m_pVS->WriteBuffer(0, mat);

	//m_pPlayer->Draw(viewMatrix, projectionMatrix);

	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);

	//m_pobjcamera->Draw();


	//オブジェクト
	m_pObjectMng->Draw(m_pCamera[CAM_OBJ]->GetViewMatrix(), m_pCamera[CAM_OBJ]->GetProjectionMatrix(),true);


	//Geometry用の変更行列を計算
	//ワールド行列の再計算
	world =
		DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f) *
		DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	//転置行列に変換
	world = DirectX::XMMatrixTranspose(world);
	//XMMATRIX型からXMFLOAT4X4に変換して格納
	DirectX::XMStoreFloat4x4(&mat[0], world);

	//Geometory用の変換行列を設定
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	//モデル表示
	//Geometory::DrawBox();

	//2D表示に変換(ミニマップやUI
	SetRenderTargets(1, &m_pRTV, nullptr);



	//コインの枠表示
	m_pCoinCntUI->Draw();


	//コインが取得されていたら描画
	if (m_pCoin[0].IsCoinCollected == true)
	{
		m_pCoin[0].Draw(68.0f, 80.0f, 0.0f, 75.0f, 75.0f, 1);
	}

	if (m_pCoin[1].IsCoinCollected == true)
	{
		m_pCoin[1].Draw(180.0f, 80.0f, 0.0f, 75.0f, 75.0f, 2);
	}

	if (m_pCoin[2].IsCoinCollected == true)
	{
		m_pCoin[2].Draw(295.0f, 80.0f, 0.0f, 75.0f, 75.0f, 3);
	}

	//ゴールしたら表示（仮）
	//本当は画面遷移
	if (m_pGoal->IsGoal == true)
	{
		m_pCoinCntUI->GoalDraw();
	}

	SetRenderTargets(1, &m_pRTV, m_pDSV);

}
