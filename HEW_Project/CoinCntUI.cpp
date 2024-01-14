//===インクルード部===
#include "CoinCntUI.h"
#include "Sprite.h"
#include "Input.h"


//===コンストラクタ===
CoinCntUI::CoinCntUI() :m_pTexture(nullptr),m_pGT(nullptr)
{

	m_pTexture = new Texture();
	m_pGT = new Texture();//ゴールの仮処理のため後ほど消すこと

	//それぞれの.pngが開けなかった時メッセージボックスを表示する
	if (FAILED(m_pTexture->Create("Assets/Texture/coin_waku.png")))
	{
		MessageBox(NULL, "000CoinUI.png", "Error", MB_OK);
	}

	m_pGT = new Texture();
	if (FAILED(m_pGT->Create("Assets/Texture/clear.png")))//ゴールの仮処理のため後ほど消すこと	
	{													  //ゴールの仮処理のため後ほど消すこと
		MessageBox(NULL, "000CoinUI.png", "Error", MB_OK);//ゴールの仮処理のため後ほど消すこと
	}													  //ゴールの仮処理のため後ほど消すこと
}

//===デストラクタ===
CoinCntUI::~CoinCntUI()
{
	if (m_pGT)			//ゴールの仮処理のため後ほど消すこと
	{					//ゴールの仮処理のため後ほど消すこと
		delete m_pGT;	//ゴールの仮処理のため後ほど消すこと
		m_pGT = nullptr;//ゴールの仮処理のため後ほど消すこと
	}					//ゴールの仮処理のため後ほど消すこと

	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===更新===
void CoinCntUI::Update()
{

}

//===描画===
void CoinCntUI::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		180.0f, 80.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -75.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::Draw();
}

void CoinCntUI::GoalDraw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		180.0f, 80.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -75.0f));
	Sprite::SetTexture(m_pGT);
	Sprite::Draw();
}
