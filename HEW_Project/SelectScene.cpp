#include "SelectScene.h"
#include "Input.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/stageselect.png"

SelectScene::SelectScene(SceneManager* pSceneManager)
	: m_pTexture(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pSceneManager(pSceneManager)	// メンバ変数を設定
{
	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "ステージ選択", "Error", MB_OK);
	}

	// カーテンフェードの取得
	m_pFade = new Fade(m_pCurtainUI);

	m_pCurtainUI = new CurtainUI();
}

SelectScene::~SelectScene()
{
	if (m_pFade)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pCurtainUI)
	{
		delete m_pCurtainUI;
		m_pCurtainUI = nullptr;
	}

	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void SelectScene::Update()
{
	m_pCurtainUI->Update();

	m_pFade->Update();
	if (IsKeyTrigger('O'))
		m_pFade->Start(true, 2.0f);// フェードイン
	if (IsKeyTrigger('P'))
		m_pFade->Start(false, 1.0f);// フェードアウト

	if (IsKeyTrigger('1'))
	{
		//		m_pFade->Start(true, 2.0f);// フェードイン
		m_pSceneManager->ChangeScene(SceneManager::SCENE_TUTORIAL);	// ゲームシーンに移る
	}
	if (IsKeyTrigger('2'))
	{
		//		m_pFade->Start(true, 2.0f);// フェードイン
		m_pSceneManager->ChangeScene(SceneManager::SCENE_TUTORIAL);	// ゲームシーンに移る
	}
	if (IsKeyTrigger('3'))
	{
		//		m_pFade->Start(true, 2.0f);// フェードイン
		m_pSceneManager->ChangeScene(SceneManager::SCENE_TUTORIAL);	// ゲームシーンに移る
	}
}

void SelectScene::Draw()
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
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -670.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();

	m_pFade->Draw();
	m_pCurtainUI->StageCurtainDraw();
}
