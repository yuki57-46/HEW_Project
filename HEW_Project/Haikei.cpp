//===インクルード部===
#include "Haikei.h"
#include "Sprite.h"
#include "Input.h"
#include "Defines.h"


//===コンストラクタ===
Haikei::Haikei()
	: m_pTexture(nullptr)
	, m_pPS(nullptr)
{

	m_pTexture = new Texture();

	//それぞれの.pngが開けなかった時メッセージボックスを表示する
	if (FAILED(m_pTexture->Create("Assets/backyard_pings/sougen.png")))
	{
		MessageBox(NULL, "Haikei.png", "Error", MB_OK);
	}

	m_pPS = new PixelShader();
	if (FAILED(m_pPS->Load("Assets/Shader/PS_Sprite.cso")))
	{
		MessageBox(NULL, "[Haikei] PS_Sprite", "Error", MB_OK);
	}
	
}

//===デストラクタ===
Haikei::~Haikei()
{
	
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===更新===
void Haikei::Update()
{
	
}


void Haikei::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//ワールド行列はXとYのみを考慮して作成
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		620.0f, 320.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//単体行列を設定
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//プロジェクション行列には2Dとして表示するための行列を設定
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//スプライトの設定
	Sprite::SetPixelShader(m_pPS);
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetOffset(DirectX::XMFLOAT2(0.0f, 0.0f));
	Sprite::SetSize(DirectX::XMFLOAT2(1000.0f, -570.0f));
	Sprite::SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}

