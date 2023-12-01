//===インクルード部===
#include "CoinCntUI.h"
#include "Sprite.h"
#include "Input.h"


//===コンストラクタ===
CoinCntUI::CoinCntUI() :m_pTexture(nullptr)
{
	m_pTexture = new Texture();

	//それぞれの.pngが開けなかった時メッセージボックスを表示する
	if (FAILED(m_pTexture->Create("Assets/UI/000CoinUI.png")))
	{
		MessageBox(NULL, "000CoinUI.png", "Error", MB_OK);
	}
	/*if (FAILED(m_pTexture->Create("Assets/UI/001CoinUI.png")))
	{
		MessageBox(NULL, "001CoinUI.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture->Create("Assets/UI/010CoinUI.png")))
	{
		MessageBox(NULL, "010CoinUI.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture->Create("Assets/UI/100CoinUI.png")))
	{
		MessageBox(NULL, "011CoinUI.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture->Create("Assets/UI/011CoinUI.png")))
	{
		MessageBox(NULL, "100CoinUI.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture->Create("Assets/UI/101CoinUI.png")))
	{
		MessageBox(NULL, "101CoinUI.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture->Create("Assets/UI/110CoinUI.png")))
	{
		MessageBox(NULL, "110CoinUI.png", "Error", MB_OK);
	}
	if (FAILED(m_pTexture->Create("Assets/UI/111CoinUI.png")))
	{
		MessageBox(NULL, "111CoinUI.png", "Error", MB_OK);
	}*/
}

//===デストラクタ===
CoinCntUI::~CoinCntUI()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===更新===
void CoinCntUI::Update()
{
	if (IsKeyPress('1'))
	{
		m_pTexture->Create("Assets/UI/000CoinUI.png");
	}
	if (IsKeyPress('2'))
	{
		m_pTexture->Create("Assets/UI/001CoinUI.png");
	}
	if (IsKeyPress('3'))
	{
		m_pTexture->Create("Assets/UI/010CoinUI.png");
	}
	if (IsKeyPress('4'))
	{
		m_pTexture->Create("Assets/UI/100CoinUI.png");
	}
	if (IsKeyPress('5'))
	{
		m_pTexture->Create("Assets/UI/011CoinUI.png");
	}
	if (IsKeyPress('6'))
	{
		m_pTexture->Create("Assets/UI/101CoinUI.png");
	}
	if (IsKeyPress('7'))
	{
		m_pTexture->Create("Assets/UI/110CoinUI.png");
	}
	if (IsKeyPress('8'))
	{
		m_pTexture->Create("Assets/UI/111CoinUI.png");
	}
}

//===描画===
void CoinCntUI::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		150.0f, 80.0f, 0.0f);

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
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -300.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}