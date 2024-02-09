#include "SceneResult.h"
#include "Input.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "SceneManager.hpp"

#define FILENAME "Assets/Texture/anmaku_usiro.png"

SceneResult::SceneResult(SceneManager* pSceneManager)
	: m_pBGTexture(nullptr)
	, m_pClearIcon(nullptr)
	, m_pFPIcon(nullptr)
	, m_pNextIcon(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_pSceneManager(pSceneManager)	// メンバ変数を設定
	, m_pPS(nullptr)
{
	m_pCurtainUI = new CurtainUI();
	m_pBGTexture = new Texture();
	if (FAILED(m_pBGTexture->Create(FILENAME)))
	{
		MessageBox(NULL, "リザルト背景", "Error", MB_OK);
	}
	m_pClearIcon = new Texture();
	if (FAILED(m_pClearIcon->Create("Assets/Texture/clear.png")))
	{
		MessageBox(NULL, "リザルトclear", "Error", MB_OK);
	}
	m_pFPIcon = new Texture();
	if (FAILED(m_pFPIcon->Create("Assets/Texture/clear_1stperformance.png")))
	{
		MessageBox(NULL, "リザルト1発クリア", "Error", MB_OK);
	}
	m_pNextIcon = new Texture();
	if (FAILED(m_pNextIcon->Create("Assets/Texture/nextperformance.png")))
	{
		MessageBox(NULL, "リザルト", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("./Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "Result Pixel Shader", "Error", MB_OK);
	}

	// カーテンフェードの取得
	m_pFade = new Fade(m_pCurtainUI);
}

SceneResult::~SceneResult()
{
	if (m_pFade)
	{
		delete m_pFade;
		m_pFade = nullptr;
	}
	if (m_pPS)
	{
		delete m_pPS;
		m_pPS = nullptr;
	}
	if (m_pCurtainUI)
	{
		delete m_pCurtainUI;
		m_pCurtainUI = nullptr;
	}
	if (m_pNextIcon)
	{
		delete m_pNextIcon;
		m_pNextIcon = nullptr;
	}
	if (m_pFPIcon)
	{
		delete m_pFPIcon;
		m_pFPIcon = nullptr;
	}
	if (m_pClearIcon)
	{
		delete m_pClearIcon;
		m_pClearIcon = nullptr;
	}
	if (m_pBGTexture)
	{
		delete m_pBGTexture;
		m_pBGTexture = nullptr;
	}
}

void SceneResult::Update()
{
	if (IsKeyTrigger(VK_RETURN))
	{
		m_pFade->Start(true, 1.0f);// フェードイン
		m_pSceneManager->ChangeScene(SceneManager::SCENE_TITLE);	// タイトルに戻る
	}
}

void SceneResult::BGDraw()
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
	Sprite::SetTexture(m_pBGTexture);
	Sprite::Draw();
}

void SceneResult::ClearDraw()
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
	Sprite::SetTexture(m_pClearIcon);
	Sprite::Draw();
}
void SceneResult::FPDraw()
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
	Sprite::SetTexture(m_pFPIcon);
	Sprite::Draw();
}
void SceneResult::NextDraw()
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
	Sprite::SetTexture(m_pNextIcon);
	Sprite::Draw();
}
