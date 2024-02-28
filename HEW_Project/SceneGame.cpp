#include "SceneGame.h"
#include "Geometory.h"
#include "Input.h"
#include <DirectXMath.h>
#include "Input.h"

#define FADE_TEST 0

SceneGame::SceneGame(int selectNum)
	: m_pSound(nullptr)
	, m_pSourceVoice(nullptr)
	, m_pVS(nullptr)
	, m_pCamera{ nullptr, nullptr,nullptr }
	, m_pobjcamera(nullptr)
	, m_pRTV(nullptr)
	, m_pDSV(nullptr)
	, m_pUI(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pHaikei(nullptr)
	, m_pCoinCntUI(nullptr)
	, m_pCoin(nullptr)
	, m_pGoal(nullptr)
	, m_pBackShadow(nullptr)
	, m_pObjectMng(nullptr)
	, m_pFade(nullptr)
	, m_pCurtain(nullptr)
	, m_pGoalTecture(nullptr)
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
	m_pHaikei = new Haikei();

	//ゴール
	m_pGoal = new Goal();
	//カーテン
	m_pCurtainUI = new CurtainUI();
	m_pCurtain = new Curtain();



	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "ERROR", MB_OK);
	}

	m_pCamera[CAM_OBJ] = new CameraObject(m_pobjcamera);
	m_pCamera[CAM_DEBUG] = new CameraDebug();
	m_pCamera[CAM_SHADOW] = new CameraShadow();

	m_pBackShadow = new BackShadow;

	m_pObjectMng = new ObjectMng(selectNum);
	//m_pDCamera = new CameraDebug();

#if FADE_TEST
	m_pFade = new Fade(m_pCurtainUI);
#endif

	// ゴール用
	m_pGoalTecture = new Texture();
	if (m_pGoalTecture->Create("Assets/Texture/clear.png"))
	{
		MessageBox(NULL, "clear.pngの読み込みエラー", "Error", MB_OK);
	}
	// ゲームオーバー用
	m_pDeadTexture = new Texture();
	if (m_pDeadTexture->Create("Assets/Texture/gameover.png"))
	{
		MessageBox(NULL, "gameover.pngの読み込みエラー", "Error", MB_OK);
	}
	m_pPS = new PixelShader();
	if (m_pPS->Load("Assets/Shader/PS_Sprite.cso"))
	{
		MessageBox(NULL, "[Coin.cpp] Failed to load Pixcel Shader", "Error", MB_OK);
	}

	m_pBackShadow->SetShadowCamera(m_pCamera[CAM_SHADOW]);
	m_pSound = LoadSound("Assets/Sound/BGM/Ge-musi-nnA_Muto.wav", true); // サウンドファイルの読み込み

	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);
	m_pBackShadow->SetShadowCamera(m_pCamera[CAM_SHADOW]);
	m_pSourceVoice = PlaySound(m_pSound); // サウンドの再生
}

SceneGame::~SceneGame()
{
#if FADE_TEST
	if (m_pFade)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}
#endif
	if (m_pHaikei)
	{
		delete m_pHaikei;
		m_pHaikei = nullptr;
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
	if (m_pCurtain)
	{
		delete m_pCurtain;
		m_pCurtain = nullptr;
	}
	if (m_pGoalTecture)
	{
		delete m_pGoalTecture;
		m_pGoalTecture = nullptr;
	}
	if (m_pDeadTexture)
	{
		delete m_pDeadTexture;
		m_pDeadTexture = nullptr;
	}
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	m_pSourceVoice->Stop();
}

void SceneGame::Update(SceneManager* pSceneManager, float tick)
{
	if (m_pobjcamera != nullptr)
		{
			m_pobjcamera->SetCamera(m_pCamera[CAM_SHADOW]);	// 例外スロー発生場所
		}

	m_pBackShadow->Update(tick);

	//m_pObjectMng->SetPlayer(m_pPlayer);
	m_pobjcamera->SetCamera(m_pCamera[CAM_OBJ]);

	m_pCamera[CAM_OBJ]->Update();
	m_bGoal = m_pGoal->GetGoal();
	// ゴールしたらシーンを切り替えてリザルトシーンに遷移する
	if (m_bGoal == true)
	{
		pSceneManager->SetNextScene(SCENE_RESULT);
	}

	//オブジェクト
	m_pobjcamera->SetCamera(m_pCamera[CAM_DEBUG]);
	m_pObjectMng->Update(tick);
	//m_pObject2D->Update();
	m_pCoinCntUI->Update();
	m_pCurtainUI->Update();

#if FADE_TEST
	m_pFade->Update();
	if (IsKeyTrigger('O'))
		m_pFade->Start(true, 2.0f);// フェードイン
	if (IsKeyTrigger('P'))
		m_pFade->Start(false, 1.0f);// フェードアウト

	//if (IsKeyTrigger(VK_RETURN))
	//{
	//	m_pSceneManager->ChangeScene(SceneManager::SCENE_RESULT);	// タイトルに戻る
	//}
#endif

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
	//m_pScreen->Draw(m_pCamera[CAM_OBJ]->GetViewMatrix(), m_pCamera[CAM_OBJ]->GetProjectionMatrix());
	m_pHaikei->Draw();
	m_pBackShadow->Draw(m_pobjcamera, m_pObjectMng, &m_pCoin[0], &m_pCoin[1], &m_pCoin[2], m_pGoal);

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
	//カーテン
	m_pCurtain->RightDraw(m_pCamera[CAM_OBJ]->GetViewMatrix(), m_pCamera[CAM_OBJ]->GetProjectionMatrix());
	m_pCurtain->LeftDraw(m_pCamera[CAM_OBJ]->GetViewMatrix(), m_pCamera[CAM_OBJ]->GetProjectionMatrix());

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



	//ゴールしたら表示（仮）
	//本当は画面遷移
	if (m_pGoal->IsGoal == true)
	{
		//スプライトの設定
		Sprite::SetPixelShader(m_pPS);
		Sprite::SetWorld(mat[0]);
		Sprite::SetView(mat[1]);
		Sprite::SetProjection(mat[2]);
		Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f/*ここ何とかすれば出そうかなぁ*/));
		Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Sprite::SetTexture(m_pGoalTecture);
		//m_pSceneManager->SetNextScene(SCENE_RESULT);
	}

#if FADE_TEST
	m_pFade->Draw();
#endif // FADE_TEST
	m_pCurtainUI->StageCurtainDraw();

	//SetRenderTargets(1, &m_pRTV, m_pDSV);

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
}
