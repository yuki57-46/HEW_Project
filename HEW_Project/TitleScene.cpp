#include "TitleScene.h"
#include "Input.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/Title.png"

SceneTitle::SceneTitle(SceneManager* pSceneManager)
	: m_pTexture(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pSceneManager(pSceneManager)	// メンバ変数を設定
	, m_pPS(nullptr)
{
	m_pCurtainUI = new CurtainUI();

	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "Title", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("./Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "Title Pixel Shader", "Error", MB_OK);
	}

	// カーテンフェードの取得
	m_pFade = new Fade(m_pCurtainUI);
}

SceneTitle::~SceneTitle()
{
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}

	//if (m_pFade)
	//{
	//	delete m_pFade;
	//	m_pFade = nullptr;
	//}

	//if (m_pCurtainUI)
	//{
	//	delete m_pCurtainUI;
	//	m_pCurtainUI = nullptr;
	//}

	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void SceneTitle::Update()
{
//	m_pCurtainUI->Update();
//	m_pFade->Update();
	//if (IsKeyTrigger('O'))
	//	m_pFade->Start(true, 2.0f);// フェードイン
	//if (IsKeyTrigger('P'))
	//	m_pFade->Start(false, 1.0f);// フェードアウト
	if (IsKeyTrigger(VK_RETURN))
	{
		m_pFade->Start(true, 1.0f);	// フェードイン
		m_pSceneManager->ChangeScene(SceneManager::SCENE_TUTORIAL);	// 操作説明シーンに移る
	}
}

void SceneTitle::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];
	
	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			640.0f, 360.0f, 0.0f
		);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	
	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
	
	//プロジェクション行列には2Dとして表示するための行列を設定する
	//この行列で2Dぼスクリーンの大きさが決まる
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f
	);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));
	
	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}

//TitleScene.cpp
//#include "TitleScene.h"
//#include "SceneManager.hpp"
//#include "Input.h"
//
//TitleScene::TitleScene()
//	: m_pTexture(nullptr)
////	, m_pSceneManager(pSceneManager)
//{
//	m_pTexture = new Texture();
//	if (FAILED(m_pTexture->Create("Assets/Texture/Title.png")))
//	{
//		MessageBox(NULL, "Title.png", "Error", MB_OK);
//	}
//}
//
//TitleScene::~TitleScene()
//{
//	if (m_pTexture)
//	{
//		delete m_pTexture;
//		m_pTexture = nullptr;
//	}
//}
//
//void TitleScene::UpdateTitle()//SceneManager* pSceneMng)
//{
//	//シーン移動判定
//	if (IsKeyTrigger(VK_RETURN))
//	{
//		//pSceneMng->SetNextScene(SCENE_SELECT);
//	}
//
//}
//
//void TitleScene::DrawTitle()
//{
//	DirectX::XMFLOAT4X4 mat[3];
//
//	//ワールド行列はXとYのみを考慮して作成
//	DirectX::XMMATRIX world =
//		DirectX::XMMatrixTranslation(
//			640.0f, 360.0f, 0.0f
//		);
//	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
//
//	//ビュー行列は2Dだとカメラの位置があまり関係ないので、単体行列を設定
//	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());
//
//	//プロジェクション行列には2Dとして表示するための行列を設定する
//	//この行列で2Dぼスクリーンの大きさが決まる
//	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
//		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f
//	);
//	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));
//
//	//スプライトの設定
//	Sprite::SetWorld(mat[0]);
//	Sprite::SetView(mat[1]);
//	Sprite::SetProjection(mat[2]);
//	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
//	Sprite::SetTexture(m_pTexture);
//	Sprite::Draw();
//}
