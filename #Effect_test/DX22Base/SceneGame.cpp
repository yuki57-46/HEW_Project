#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "PlayerCamera.h"


SceneGame::SceneGame()
	: m_pModel(nullptr)
	, m_pVS(nullptr)
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
	, m_pUI(nullptr)
	, m_pPS(nullptr)
	, m_pEff(nullptr)
{
	// レンダーターゲット、深度バッファの設定
	RenderTarget* pRTV = GetDefaultRTV(); // デフォルトで使用しているRenderTargetViewの取得
	DepthStencil* pDSV = GetDefaultDSV(); // デフォルトで使用しているDepthStencilViewの取得
	SetRenderTargets(1, &pRTV, pDSV);  // DSVがnullだと2D表示になる
	
	m_pPlayer = new Player();
	
	m_pCamera = new PlayerCamera(m_pPlayer);

	m_pUI = new ItemUI();
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("Assets/Shader/PS_Gray_test.cso")))
	{
		MessageBox(nullptr, "PS_Model.cso", "Error", MB_OK);
	}

	m_pEff = new Effekseer_Effect();

	//m_pModel = new Model;
	//if (!m_pModel->Load("Assets/Model/Squid_LOD0.fbx", 0.025f, Model::XFlip)) // アセットのパス, 倍率, 反転
	//{
	//	MessageBox(NULL, "Model", "Error", MB_OK);	//
	//}
	//m_pModel->SetVertexShader(m_pVS);	// モデルへ読み込んだ頂点シェーダーを設定
	m_pPlayer->SetCamera(m_pCamera);
	//m_pEff->SetCamera(m_pCamera);
}

SceneGame::~SceneGame()
{
	if (m_pEff)
	{
		delete m_pEff;
		m_pEff = nullptr;
	}

	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}
	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

void SceneGame::Update(float tick)
{
	//static float rad = 0.0f;
	//DirectX::XMMATRIX Rx = DirectX::XMMatrixRotationX(rad); // radを使って回転させる
	//DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(rad);	// radを使って回転させる
	//DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(rad);	// radを使って回転させる
	//DirectX::XMMATRIX mat = Rx * Ry * Rz;
	//mat = DirectX::XMMatrixTranspose(mat);
	//DirectX::XMFLOAT4X4 fMat; // 行列の格納先
	//DirectX::XMStoreFloat4x4(&fMat, mat);
	//Geometory::SetWorld(fMat); // ボックスに変換行列を設定
	//rad += tick; // 回転角の更新

	m_pPlayer->Update();

	m_pCamera->Update();

	
	m_pEff->Update();


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
	static float rad = 0.0f;

	DirectX::XMFLOAT4X4 mat[3];

	// --- ワールド行列の計算
	//DirectX::XMMATRIX world =[ワールド行列の設定 ※ 移動、回転、拡縮を考慮すると1行では収まらない;
	DirectX::XMMATRIX world =
							DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
							DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
							DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	// --- ビュー行列の計算
	//DirectX::XMMATRIX view = [ビュー行列の設定]

	//DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
	//								DirectX::XMVectorSet(-3.0f, 4.0f, -3.0f, 0.0f),	// カメラの位置
	//								DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),	// 注視点の位置
	//								DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f)	// アップベクトル
	//							);
	//view = DirectX::XMMatrixTranspose(view);
	//// [XMMATRIX型からXMFLOAT4X4型(mat[1])へ変換して格納
	//DirectX::XMStoreFloat4x4(&mat[1], view);

	// --- プロジェクション行列の計算
	// DirectX::XMMATRIX proj; プロジェクション行列
	//DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(5.0f, 16.0 / 9.0f, 10.0f, 500.0f);
	//proj = DirectX::XMMatrixTranspose(proj);
	//// [XMMATRIX型からXMFLOAT4X4型(mat[2])へ変換して格納
	//DirectX::XMStoreFloat4x4(&mat[2], proj);
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();



	// --- 行列をシェーダーへ設定
	m_pVS->WriteBuffer(0, mat);

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);
	
	
	m_pPlayer->Draw();
	// モデル表示
	if (m_pModel)
	{
		m_pModel->Draw();
	}

	m_pEff->Draw();
	
	// Geometory用の変換行列の計算
	//[ワールド行列の設定]
	world = DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f) *
			DirectX::XMMatrixRotationX(rad) * DirectX::XMMatrixRotationY(rad) * DirectX::XMMatrixRotationZ(rad) *
			DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);
	//[転置行列に変換
	world = DirectX::XMMatrixTranspose(world);
	//[XMMATRIX型(world)からXMFLOAT4X4(mat[0])へ変換して格納
	DirectX::XMStoreFloat4x4(&mat[0], world);
	// Geometory用の変換行列の設定
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);

	Geometory::DrawBox();

	SetRenderTargets(1, &pRTV, nullptr);
	m_pUI->Draw();

	
}
