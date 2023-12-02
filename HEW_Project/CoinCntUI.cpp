//===�C���N���[�h��===
#include "CoinCntUI.h"
#include "Sprite.h"
#include "Input.h"


//===�R���X�g���N�^===
CoinCntUI::CoinCntUI() :m_pTexture(nullptr)
{
	m_pTexture = new Texture();

	//���ꂼ���.png���J���Ȃ����������b�Z�[�W�{�b�N�X��\������
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

//===�f�X�g���N�^===
CoinCntUI::~CoinCntUI()
{
	if (m_pTexture)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}

//===�X�V===
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

//===�`��===
void CoinCntUI::Draw()
{
	DirectX::XMFLOAT4X4 mat[3];

	//���[���h�s���X��Y�݂̂��l�����č쐬
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
		150.0f, 80.0f, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	//�P�̍s���ݒ�
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixIdentity());

	//�v���W�F�N�V�����s��ɂ�2D�Ƃ��ĕ\�����邽�߂̍s���ݒ�
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f, 720.0f, 0.0f, 0.1f, 10.0f);
	DirectX::XMStoreFloat4x4(&mat[2], DirectX::XMMatrixTranspose(proj));

	//�X�v���C�g�̐ݒ�
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(300.0f, -300.0f));
	Sprite::SetTexture(m_pTexture);
	Sprite::Draw();
}