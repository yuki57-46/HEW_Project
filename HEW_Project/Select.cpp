//Select.cpp
#include "Select.h"
#include "Input.h"

Select::Select()
	:m_pTexture(nullptr)
{
	m_pTexture = new Texture();
	if (FAILED(m_pTexture->Create("Assets/Texture/stageselect.png")))
	{
		MessageBox(NULL, "stageselect.png", "Error", MB_OK);
	}
}

Select::~Select()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

void Select::UpdateTitle()//SceneManager* pSceneMng)
{
	//シーン移動判定
	if (IsKeyTrigger(VK_RETURN))
	{
		//pSceneMng->SetNextScene(STAGE_ONE);
	}

}

void Select::DrawTitle()
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
	Sprite::SetSize(DirectX::XMFLOAT2(1280.0f, -720.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}
