#include "ShadowP.h"
#include "Input.h"
DirectX::XMFLOAT3 PMinBound = DirectX::XMFLOAT3(-0.15f, -0.1f, -0.3f);  //���E�̍ŏ��l
DirectX::XMFLOAT3 PMaxBound = DirectX::XMFLOAT3(0.2f, 0.1f, 0.5f);     //�ő�l
ShadowP::ShadowP()
	: m_pos(-3.5f, 0.5f, 10.0f)
	, m_oldPos(0.0f, 0.0f, 10.0f)
	, IsUse(false)
	, m_Jump(false)
	, m_moveY(0.0f)
	, m_JumpY(0.5f)
{
	m_pModel = new Model;
	//if (!m_pModel->Load("Assets/Model/Golem//Golem.FBX"))
	//{
	//	MessageBox(NULL, "�ǂݍ��񂾃t�@�C����", "Error", MB_OK);
	//}

	 //���f���̓ǂݍ��ݏ���
	if (!m_pModel->Load("Assets/Model/Player/kage.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}
	/*if (!m_pModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}*/
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);

	minBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);
	maxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);

	SetBounds(minBound, maxBound);
}


ShadowP::~ShadowP()
{
	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}
}

void ShadowP::Update()
{
	m_oldPos = m_pos;
	if (IsUse == true)
	{
		m_pos.x -= 0.015;
	}
	else if (IsUse == false)
	{
		m_pos.x += 0.015;
	}

	m_pos.y -= 0.05f;
	m_pos.y += m_moveY;
	if (m_moveY > 0.0f)
	{
		m_moveY -= 0.2;
	}

	//if (m_Jump == true)
	//{
	//	m_moveY = 1.2f;
	//	//m_pos.y += m_moveY;
	//	m_Jump = false;
	//}
	//m_moveY�͏�ɉ������։�������
	//m_moveY�̑��x�ɉ����ăv���C���[�̍��W��ς���
	//�W�����v

	
	SetBounds(PMinBound, PMaxBound);  //�ŏ��l�ƍő�l���Z�b�g
}

void ShadowP::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	//---���[���h�s��̌v�Z
	DirectX::XMMATRIX MT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	//DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(1.0f, 2.5f, 1.0f);
	DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(0.7f, 1.2f, 0.7f);
	DirectX::XMMATRIX world = MS * MT;
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = viewMatrix; // �^����ꂽ viewMatrix ���g��
	mat[2] = projectionMatrix; // �^����ꂽ projectionMatrix ���g��
	//----�s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);    //�z��̐擪�A�h���X���w�肵�āA�܂Ƃ߂ĕϊ��s���n��
	m_pModel->Draw();
}

void ShadowP::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 ShadowP::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//pos�ɍŏ��l�A�ő�l�𑫂��ē����蔻������炷
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

void ShadowP::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void ShadowP::ShadowPPosY()
{
	m_pos.y = m_oldPos.y;
}

void ShadowP::oldX()
{
	m_pos.x = m_oldPos.x;
}

float ShadowP::ShadowPPosX()
{
	return m_pos.x;
}

DirectX::XMFLOAT3 ShadowP::NowPos()
{
	return m_pos;
}


void ShadowP::ShadowPPos()
{
	m_pos = m_oldPos;
}

void ShadowP::Use()
{
	//if (IsUse == false)
	//{
	//	IsUse = true;
	//}
	IsUse = true;

}

void  ShadowP::NotUse()
{
	//if (IsUse == true)
	//{
	//	
	//}
	IsUse = false;
}

void ShadowP::Jump()
{
	m_Jump = true;
	if (m_Jump == true)
	{
		m_moveY = 0.06f;
		//m_pos.y += m_moveY;
		m_Jump = false;
	}
}

bool ShadowP::isUse()
{
	return IsUse;
}

bool ShadowP::IsJump()
{
	return m_Jump;
}
