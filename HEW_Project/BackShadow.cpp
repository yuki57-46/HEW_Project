#include "BackShadow.h"
#include "Sprite.h"
#include "Geometory.h"

#define RTV_3D_SIZE_WIDTH	(1280.0f / 1.5f)	//3D��ԏ�̃����_�[�̕\���T�C�YX
#define RTV_3D_SIZE_HEIGHT	(-720.0f / 1.5f)	//3D��ԏ�̃����_�[�̕\���T�C�YY
#define RTV_3D_POS_WIDTH	(640.0f)			//3D��ԏ�̃����_�[�\���̌��_X
#define RTV_3D_POS_HEIGHT	(360.0f)			//3D��ԏ�̃����_�[�\���̌��_Y

int testw = 10;
int testh = 10;

BackShadow::BackShadow()
	: m_pRTVTexture(nullptr)
	, m_pRTV_BS(nullptr)
	, m_pDSV_BS(nullptr)
	, m_pCamera(nullptr)
	, m_pShadowPlayer(nullptr)
	, m_indexX(0)
	, m_indexY(0)
	, m_castPosX(0)
	, m_castPosY(0)
	, m_SPposX(0.0f)
	, m_SPposY(0.0f)
	, m_SPpos(0.0f, 0.0f, 0.0f)
	, m_alpha{0}
	, m_underAlpha{0}
	, m_Player_a{0}
	, m_sumAlpha(0)
	, m_alphaData(0)
	, m_noAlphaData(0)
	, m_collisionFlag(false)
	, m_upFlag(false)
	, m_LRcheck(false)
{
	//�����_�[�\���֘A�̊m��
	m_pRTVTexture = new Texture;
	m_pRTV_BS = new RenderTarget;
	m_pDSV_BS = new DepthStencil;

	//�����_�[�^�[�Q�b�g����
	m_pRTV_BS->Create(DXGI_FORMAT_R8G8B8A8_UNORM, 1280 / 2, 720 / 2);//�����_�[���̑傫��
	m_pDSV_BS->Create(m_pRTV_BS->GetWidth(), m_pRTV_BS->GetHeight(), true);
	SetRenderTargets(1, &m_pRTV_BS, m_pDSV_BS);

	//�e�݂̂ɕ\��������̂̊m��
	m_pShadowPlayer = new ShadowP;

	//�R�s�[�p�t�H�[�}�b�g�쐬
	m_pRTV_BS->CreateReadBuffer();

	//�e�̓����蔻��p�ϐ�������
	m_SPpos = m_pShadowPlayer->NowPos();

	//�i�s����������
	m_LRcheck = m_pShadowPlayer->isUse();
}

BackShadow::~BackShadow()
{
	if (m_pRTVTexture)
	{
		delete m_pRTVTexture;
		m_pRTVTexture = nullptr;
	}
	if (m_pRTV_BS)
	{
		delete m_pRTV_BS;
		m_pRTV_BS = nullptr;
	}
	if (m_pDSV_BS)
	{
		delete m_pDSV_BS;
		m_pDSV_BS = nullptr;
	}
	if (m_pShadowPlayer)
	{
		delete m_pShadowPlayer;
		m_pShadowPlayer = nullptr;
	}
}

/**
 * @fn
 * �e(�����_�[)�̍X�V
 * �e�v���C���[�̍X�V
 */
void BackShadow::Update()
{
	m_pShadowPlayer->Update();
}

/**
 * @fn
 * ��{����@���@�e(�����_�[)�̕`��
 * ��{�ȊO�@���@�e�̃e�N�X�`�������擾�A�����蔻��p�ϐ��̍X�V
 * @brief �e(�����_�[)�̕`��
 * @param (Player* Player) �v���C���[�̃|�C���^
 * @param (ObjectMng* Obj) �I�u�W�F�̃|�C���^
 * @sa �Q�Ƃ��ׂ��֐��������΃����N���\���
 * @detail 3D��Ԃɂ�����̂�\���������Ƃ��́u�����𑝂₷�v����
 */
void BackShadow::Draw(ObjectMng* Obj)
{
	//�[�x�o�b�t�@�̃N���A
	//m_pDSV_BS->Clear();

	//�����_�[�̐F�ݒ肵�A���̐F�ŉ�ʃN���A
	float color[4] = { 1.0f, 0.5f, 1.0f, 0.1f };
	m_pRTV_BS->Clear(color);
	//m_pRTV_BS->Clear();

	SetRenderTargets(1, &m_pRTV_BS, m_pDSV_BS);//�����_�[�̐ݒ�

	//�����ɕ\�����������̂������Ă���
	DirectX::XMFLOAT4X4 viewMatrix = m_pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 projectionMatrix = m_pCamera->GetProjectionMatrix();
	Obj->Draw(viewMatrix, projectionMatrix);
	m_pShadowPlayer->Draw(viewMatrix, projectionMatrix);


	RenderTarget* pRTV;
	pRTV = GetDefaultRTV();

	SetRenderTargets(1, &pRTV, nullptr);

	//�����_�[�^�[�Q�b�g�̐F���ǂݎ��
	m_pRTV_BS->Read([&](const void* colorData, UINT width, UINT height) {
		struct Color {
			BYTE r, g, b, a;
		};

		//�����_�[�����蔻��p�ϐ��X�V
		m_SPpos = m_pShadowPlayer->NowPos();				//�e�̍��W������
		m_SPposX = ((m_SPpos.x - 5.0f) / 10.0f) * (-1);		//X���������_�[�̃E�B���h�E���W�ɍ��킹�ĕϊ�
		m_SPposY = ((m_SPpos.y - 3.0f) / 6.0f) * (-1);		//Y���������_�[�̃E�B���h�E���W�ɍ��킹�ĕϊ�
		
		//�L���X�g�p
		m_castPosX = static_cast<int>(m_SPposX * width);
		m_castPosY = static_cast<int>(m_SPposY * height);

		//�v���C���[�̈ʒu�ƃX�N���[������̍��W�̍������A�X�N���[���̉����Ŋ���Ɣz��̃C���f�b�N�X�����߂���
		//�e�̃v���C���[�̓����_�[�ɂ̂ݕ\�����Ă���->�����_�[�����e�v���C���[�̃E�B���h�E��
		//�������A����̃v���C���[�̈ʒu�ƃX�N���[������̍��W�̍����͂Ȃ����߂��̂܂܂ŁA���A���̒l��0�`1�ɂ��Ă��邽�ߊ|���Z���g�p
		//(��)(int)((m_pos.x - screenPos.x) / screenWidth)
		m_indexX = m_castPosX;
		m_indexY = m_castPosY;

		const Color* pData = reinterpret_cast<const Color*>(colorData);
		//m_Player_a = pData[m_indexY * width + m_indexX].a;	//�v���C���[�̈ʒu�̃��l��������
		//m_alpha[0] = pData[m_indexY * width + m_indexX + 11].a;

		//if (m_alpha[0] > 128)
		//{
		//	m_pShadowPlayer->Use();//�G���[237-392
		//}

		//�i�s�����`�F�b�N
		m_LRcheck = m_pShadowPlayer->isUse();

		for (int h = 0; h < height; ++h)
		{
			for (int w = 0; w < width; ++w)
			{
				m_alphaData = 1;
				m_noAlphaData = 1;
				m_sumAlpha = 0;
				for (int j = 0; j < 5; j++)
				{
					for (int i = 0; i < 5; i++)
					{
						if (m_LRcheck == false)
						{
							m_alpha = pData[(m_indexY - j) * width + m_indexX + 10 + i].a;	//�����_�[�E�B���h�E�̃��l�����ォ�������Ă�
						}
						else
						{
							m_alpha = pData[(m_indexY - j) * width + m_indexX - 10 - i].a;
						}

						if (m_alpha > 200)
						{
							m_sumAlpha += m_alpha;
							m_alphaData++;
						}
						if (m_alpha < 100)
						{
							m_noAlphaData++;
						}
					}
				}
				
				if (ShadowCollision(m_sumAlpha, m_alphaData, m_noAlphaData) || ShadowEdgeCollision(h, width))
				{
					m_collisionFlag = true;
					break;
				}
			}
			if (m_collisionFlag)
			{
				m_collisionFlag = false;
				break;
			}
		}
		m_underAlpha = pData[(m_indexY + 2) * width + m_indexX + 10].a;
		ShadowUnderCollision(m_underAlpha);
	});

	DirectX::XMFLOAT4X4 mat[3];

	//���[���h�s���X��Y�̂ݍl��
	DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(RTV_3D_POS_WIDTH, RTV_3D_POS_HEIGHT, 0.0f);

	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));

	mat[1] = m_pCamera->GetShadowViewMatrix();
	mat[2] = m_pCamera->GetShadowProjectionMatrix();

	//�X�v���C�g�̐ݒ�
	Sprite::SetWorld(mat[0]);
	Sprite::SetView(mat[1]);
	Sprite::SetProjection(mat[2]);
	Sprite::SetSize(DirectX::XMFLOAT2(RTV_3D_SIZE_WIDTH, RTV_3D_SIZE_HEIGHT));
	Sprite::SetTexture(m_pRTV_BS);
	Sprite::Draw();
}

/**
 * @fn
 * �����_�[�p�J�����̃Z�b�g
 */
void BackShadow::SetShadowCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

/**
 * @fn
 * ���l���g���������蔻��
 * @brief �e(�����_�[)�̓����蔻��
 * @param (BYTE alpha)	�����_�[��ʂ̃��l(��s�N�Z����)
 * @param (int h)�@		�����_�[��ʂ̍����̎Q�ƒl(��s�N�Z������)
 * @param (int w)�@		�����_�[��ʂ̕��̎Q�ƒl(��s�N�Z������)
 * @param (UINT width)	�����_�[��ʂ̕�
 * @return true or false(�����Ȃ��Ȃ�false�ŕԂ�)
 * @detail ���͂܂��`�悪���邩���Ă邾��
 */
bool BackShadow::ShadowCollision(int sumAlpha, int cntAlpha, int noAlpha)
{
	//�ϐ��m�F�v���O����
	//if (cntAlpha > 13 && noAlpha > 10)
	//{
	//	int a = sumAlpha / (cntAlpha + noAlpha);
	//	int b = 0;
	//}

	// ���E�̃��l�̎Q��
	if (sumAlpha / cntAlpha > 240 && cntAlpha > 25)
	{// ��
		m_pShadowPlayer->Use();
		return true;
	}
	if (sumAlpha / (cntAlpha + noAlpha) > 128 && cntAlpha > 13 && noAlpha > 10)
	{// �K�i
		m_pShadowPlayer->Jump();
		return true;
	}

	return false;
}

bool BackShadow::ShadowUnderCollision(BYTE underAlpha)
{
	// �����̃��l�Q��
	if (underAlpha > 240)
	{
		m_pShadowPlayer->SetFooting(true);
		return true;
	}
	else
	{
		m_pShadowPlayer->SetFooting(false);
	}
	return false;
}

bool BackShadow::ShadowEdgeCollision(int h, UINT width)
{
	if (h * width + 2 == m_indexY * width + m_indexX)
	{// ���̉�ʒ[
		m_pShadowPlayer->Use();
		return true;
	}
	if (h * width + width - 1 == m_indexY * width + m_indexX)
	{// �E�̉�ʒ[
		m_pShadowPlayer->Use();
		return true;
	}
	return false;
}