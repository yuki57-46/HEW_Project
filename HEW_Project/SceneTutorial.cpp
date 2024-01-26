#include "SceneTutorial.h"
#include "Input.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/gameover.png"	// 確認用のテクスチャ

SceneTutorial::SceneTutorial(SceneManager *pSceneManager)
	: m_pTexture(nullptr)
	, m_pSceneManager(pSceneManager)	// メンバ変数を設定
{
	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "Title", "Error", MB_OK);
	}
}

SceneTutorial::~SceneTutorial()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void SceneTutorial::Update()
{
	if (IsKeyTrigger(VK_RETURN))
	{
		m_pSceneManager->ChangeScene(SceneManager::SCENE_GAME);	// ゲームシーンに移る
	}
}

void SceneTutorial::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		0.8f, 0.8f, 0.0f);		// ワールド行列(必要に応じて変数を増やしたり、複数処理したり)
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	// ビュー行列は2Dカメラだとカメラの位置があまり関係ないので、単位行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	// プロジェクション行列には2Dとして表示するための行列を設定する
	//	この行列で2Dのスクリーンの大きさが決まる
	DirectX::XMMATRIX proj =
		DirectX::XMMatrixOrthographicOffCenterLH(
			//			-400.0f, 400.0f, 180.0f, -150.0f, 0.1f, 10.0f);// 平行投影を設定
			-150.0f, 150.0f, 120.0f, -90.0f, 0.1f, 10.0f
			/*0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f*/);// 平行投影を設定
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	// スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -300.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}
