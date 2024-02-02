#include "SceneTutorial.h"
#include "Input.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

// ファイルパス
#define FILENAME1 "Assets/Texture/gametutorial.png"	// 操作画面
#define FILENAME2 "Assets/Texture/enter.png"		// start push Enter文字
#define FILENAME3 "Assets/Texture/message.png"		// 操作一覧の文字

SceneTutorial::SceneTutorial(SceneManager *pSceneManager)
	: m_pTexture1(nullptr)
	, m_pTexture2(nullptr)
	, m_pTexture3(nullptr)
	, m_pSceneManager(pSceneManager)	// メンバ変数を設定
	, m_pPS(nullptr)
{
	m_pTexture1 = new Texture();
	if (FAILED(m_pTexture1->Create(FILENAME1)))
	{
		MessageBox(NULL, "Title", "Error", MB_OK);
	}

	m_pTexture2 = new Texture();
	if (FAILED(m_pTexture2->Create(FILENAME2)))
	{
		MessageBox(NULL, "Enter message", "Error", MB_OK);
	}

	m_pTexture3 = new Texture();
	if (FAILED(m_pTexture3->Create(FILENAME3)))
	{
		MessageBox(NULL, "Message", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("./Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "Tutorial Pixel Shader", "Error", MB_OK);
	}
}

SceneTutorial::~SceneTutorial()
{
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pTexture1)
	{
		delete m_pTexture1;
		m_pTexture1 = nullptr;
	}
	if (m_pTexture2)
	{
		delete m_pTexture2;
		m_pTexture2 = nullptr;
	}
	if (m_pTexture3)
	{
		delete m_pTexture3;
		m_pTexture3 = nullptr;
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

	// XとYのみ考慮したワールド行列
	DirectX::XMMATRIX world =
		DirectX::XMMatrixTranslation(
			640.0f, 360.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	// 2D用のビュー行列（単位行列）
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	// 2D表示のためのプロジェクション行列
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	// スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetPixelShader(m_pPS);
//	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));

	// テクスチャ1の描画
	{
		Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
		Sprite::SetTexture(m_pTexture1);
		Sprite::Draw();
	}

	// テクスチャ2の描画
	world = DirectX::XMMatrixTranslation(
		360.0f, 600.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	Sprite::SetWorld(mat[0]);

	{
		Sprite::SetSize(DirectX::XMFLOAT2(360.0f, -120.0f));
		Sprite::SetTexture(m_pTexture2);
		Sprite::Draw();
	}

	// テクスチャ3の描画
	world = DirectX::XMMatrixTranslation(
		100.0f, 40.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	Sprite::SetWorld(mat[0]);

	{
		Sprite::SetSize(DirectX::XMFLOAT2(140.0f, -60.0f));
		Sprite::SetTexture(m_pTexture3);
		Sprite::Draw();
	}
}
