#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"
#include "CameraPlayer.h"
#include "Setting.h"

SceneGame::SceneGame()
	: m_pModel(nullptr)
	, m_pVS(nullptr)
	, m_pCamera(nullptr)
	, m_pPlayer(nullptr)
	, m_pUI(nullptr)
{	
	// カメラの読み込み
//	m_pCamera = new CameraDebug;
	m_pPlayer = new Player();
	m_pCamera = new CameraPlayer(m_pPlayer);
	m_pPlayer->SetCamera(m_pCamera);
	m_pUI = new ItemUI();

	// モデルの読み込み
	m_pModel = new Model();
	// ここでアニメーションさせたいもののモデルを指定する
	// 基本的にファイルパスはmixamoフォルダに使うモデルを入れておく
	if(!m_pModel->Load(MODEL_PATH, MODEL_SCALE, MODEL_FLIP))	// ファイル指定、倍率指定、反転指定
	{
		MessageBox(NULL, "アニメーションモデル", "Error", MB_OK);	// エラーメッセージの表示
	}
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));	// 12/05 追加

	// ここはアニメーションさせるものをパスに指定させる
	// モデルとアニメーションで分かれている場合、パスの指定を"Assets/Model/Animation/アニメーション用のfbx
	// という風に切り替えておくと分かりやすい。　その際、Animationフォルダに動くモデルを入れておく
	Model::AnimeNo anime = m_pModel->AddAnimation(ANIME_PATH);
	m_pModel->Play(anime, true);

	// 頂点シェーダーの読み込み
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	// 12/05 アニメーションの際は不要なので削除
	//m_pModel->SetVertexShader(m_pVS);	// モデルへ読み込んだ頂点シェーダーを設定

	// レンダーターゲット、深度バッファの設定
	RenderTarget* pRTV = GetDefaultRTV();	// デフォルトで使用しているRenderTargetViewの取得
	DepthStencil* pDSV = GetDefaultDSV();	// デフォルトで使用しているDepthStencilViewの取得
	SetRenderTargets(1, &pRTV, pDSV);		// DSVがnullだと2D表示になる

}

SceneGame::~SceneGame()
{
	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}

	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}

	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}

	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pPlayer)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}

}

void SceneGame::Update(float tick) 
{
	//地面として変形させる

	//グリッドよりも下に来るように移動
	DirectX::XMMATRIX T = 
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);

	//地面となるように、前後左右に広く、上下に狭くする
	DirectX::XMMATRIX S = 
		DirectX::XMMatrixScaling(10.0f, 0.1f, 10.0f);

	DirectX::XMMATRIX mat = S * T;	//すべての行列を一つにまとめる計算

	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;				//行列の格納先
	DirectX::XMStoreFloat4x4(&fMat, mat);	//matをfMatに格納する処理
	Geometory::SetWorld(fMat);

	m_pModel->Step(tick);
	m_pPlayer->Update();
	m_pCamera->Update();
}

void SceneGame::Draw()
{
	static float rad = 0.0f;

	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列の計算
	DirectX::XMMATRIX world = 
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *		// 拡大縮小
		DirectX::XMMatrixRotationX(rad) *					// 回転X
		DirectX::XMMatrixRotationY(rad) *					// 回転Y
		DirectX::XMMatrixRotationZ(rad) *					// 回転Z
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);	// 移動

	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	// 行列をシェーダーへ設定
	m_pVS->WriteBuffer(0, mat);		// 配列の先頭アドレスを指定して、まとめて変換行列を渡す

	RenderTarget* pRTV = GetDefaultRTV();
	DepthStencil* pDSV = GetDefaultDSV();
	SetRenderTargets(1, &pRTV, pDSV);

	// Geometory用の変換行列を計算
	// 12/05 0から変えた
	world = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// 拡大縮小
		// Geometryの設定を変えてあげる
		DirectX::XMMatrixRotationX(rad) *					// 回転X
		DirectX::XMMatrixRotationY(rad) *					// 回転Y
		DirectX::XMMatrixRotationZ(rad) *					// 回転Z
		DirectX::XMMatrixTranslation(0.0f, -0.05f, 0.0f);	// 移動

	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	// Geometory用の変換行列を設定
	Geometory::SetWorld(mat[0]);
	Geometory::SetView(mat[1]);
	Geometory::SetProjection(mat[2]);
	Geometory::DrawBox();	//ボックスを描画

	// レンダーターゲットの再定義→2D表示に切り替える
	// 変数は3Dの物と違う変数で定義する
	RenderTarget* pRTV2D = GetDefaultRTV();
	DepthStencil* pDSV2D = GetDefaultDSV();
	SetRenderTargets(1, &pRTV2D, pDSV2D);		// 3D表示
	SetRenderTargets(1, &pRTV2D, nullptr);		// 2D表示

	// アニメーションの描画
	ShaderList::SetWVP(mat);	// 転置済みの変換行列
	m_pModel->Draw(nullptr, [this](int index)	// ラムダ式
		{
			const Model::Mesh* pMesh = m_pModel->GetMesh(index);
			const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
			ShaderList::SetMaterial(*pMaterial);

			DirectX::XMFLOAT4X4 bones[200];
			for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
			{
				// この計算はゲームつくろー「スキンメッシュの仕組み」が参考になる
				DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
					pMesh->bones[i].invOffset *
					m_pModel->GetBone(pMesh->bones[i].index)
				));
			}
			ShaderList::SetBones(bones);
		});

#ifdef _DEBUG
	if (BONE_VIER)
	{
		m_pModel->DrawBone();
	}

#endif // DEBUG
}
