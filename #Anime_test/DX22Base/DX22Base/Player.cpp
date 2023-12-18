#include "Player.h"
#include "Input.h"
#include "Sprite.h"
#include "Texture.h"

Player::Player()
	: m_pos(0.0f, 0.4f, 0.0f)
	, m_pCamera(nullptr)
	, m_pShadowTex(nullptr)
	, m_pMarkerTex(nullptr)
{
	// ���f���̓ǂݍ���
	m_pModel = new Model;
//	if (!m_pModel->Load("Assets/Model/spot/spot.fbx", 0.6f))	
//	if (!m_pModel->Load("Assets/Model/myroom.mb", 1.0f, Model::XFlip))
	if (!m_pModel->Load("Assets/Model/Golem/Golem.FBX", 0.01f))	// �t�@�C���w��A�{���w��A���]�w��
	{
		MessageBox(NULL, "Model", "Player Error", MB_OK);	// �G���[���b�Z�[�W�̕\��
	}

	// �e�̃e�N�X�`���ǂݍ���
	m_pShadowTex = new Texture();
	if (FAILED(m_pShadowTex->Create("Assets/Texture/Shadow.png")))
	{
		MessageBox(NULL, "Shadow", "Error", MB_OK);
	}

	// �}�[�J�[�̃e�N�X�`���ǂݍ���
	m_pMarkerTex = new Texture();
	if (FAILED(m_pMarkerTex->Create("Assets/Texture/Marker.png")))
	{
		MessageBox(NULL, "Marker", "Error", MB_OK);
	}

	// ���_�V�F�[�_�[�̓ǂݍ���
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);	// ���f���֓ǂݍ��񂾒��_�V�F�[�_�[��ݒ�

	m_pEffect = new Effect();
	m_pMoveEmitter = new MoveEmitter();
	m_pEffect->AddEmitter(m_pMoveEmitter);
}

Player::~Player()
{
	delete m_pMoveEmitter;
	delete m_pEffect;

	if (m_pVS)
	{
		delete m_pVS;
		m_pVS = nullptr;
	}

	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void Player::Update()
{
	bool isMove = false;
	DirectX::XMFLOAT3 camPos	= m_pCamera->GetPos();
	DirectX::XMFLOAT3 camLook	= m_pCamera->GetLook();
	DirectX::XMVECTOR vCamPos	= DirectX::XMLoadFloat3(&camPos);
	DirectX::XMVECTOR vCamLook	= DirectX::XMLoadFloat3(&camLook);

	DirectX::XMVECTOR vFront;	// �J�����̐��ʕ����̃x�N�g��
	// �J�����̈ʒu�ƒ����_���琳�ʕ����̃x�N�g�����v�Z
	vFront = DirectX::XMVectorSubtract(vCamLook, vCamPos);
	// �J�����̐��ʕ����̃x�N�g���𐳋K��
	vFront = DirectX::XMVector3Normalize(vFront);

	// �x�N�g����90����]������s����v�Z
	DirectX::XMMATRIX matRotSide = 
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90.0f));
	// ���ʕ����̃x�N�g������]�s���K�p�������x�N�g��
	DirectX::XMVECTOR vSide	
		= DirectX::XMVector3TransformCoord(vFront, matRotSide);
	// 0�ŏ�����
	DirectX::XMVECTOR vMove	
		= DirectX::XMVectorZero();

	// vMove��vFront�����Z���Đ��ʂֈړ�
	if (IsKeyPress('W')) { isMove = true; vMove = DirectX::XMVectorAdd(vMove, vFront); };
	// vMove����vFront�����Z���Č��ֈړ�
	if (IsKeyPress('S')) { isMove = true; vMove = DirectX::XMVectorSubtract(vMove, vFront); };
	// vMove��vSide�����Z���ĉE�ֈړ�
	if (IsKeyPress('D')) { isMove = true; vMove = DirectX::XMVectorAdd(vMove, vSide); };
	// vMove����vSide�����Z���č��ֈړ�
	if (IsKeyPress('A')) { isMove = true; vMove = DirectX::XMVectorSubtract(vMove, vSide); };

	vMove = DirectX::XMVectorMultiply(vMove, DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f));
	vMove = DirectX::XMVector3Normalize(vMove);		// �ړ��x�N�g���̐��K��
	vMove = DirectX::XMVectorScale(vMove, 0.1f);	// �ړ��x�N�g���̈ړ����x��ύX�i�萔�{�j

	DirectX::XMFLOAT3 move;	// �v�Z�����ړ��x�N�g���̊i�[��
	// XMVECTOR�^����XMFLOAT3�^�ɕϊ����Amove�Ɋi�[
	DirectX::XMStoreFloat3(&move, vMove);

	// ���W�̍X�V
	m_pos.x += move.x;
	m_pos.y += move.y;
	m_pos.z += move.z;

	// �G�t�F�N�g�̔���
	m_pEffect->Update();
	m_pEffect->SetPos(m_pos);
	if (!m_pEffect->IsPlay() && isMove)	// �G�t�F�N�g���Đ�����Ă��Ȃ�������
	{
		m_pEffect->Play();
	}
}

void Player::Draw()
{
	if (!m_pCamera) { return; }

	//DirectX::XMFLOAT4X4 inv; // �t�s��(inverse)�̊i�[��
	//inv = m_pCamera->GetViewMatrix();
	//// �J�����̍s���GPU�ɓn���ۂɓ]�u����Ă��邽�߁A�t�s��̌v�Z�̂��߂Ɉ�x���ɖ߂�
	//DirectX::XMMATRIX matInv = ;// inv�Ɋi�[���ꂽ�J�����s���ϊ�
	//matInv = DirectX::XMMatrixTranspose(matInv);// matInv��]�u����
	//// �ړ������͋t�s��őł������K�v���Ȃ��̂ŁA0��ݒ肵�Ĉړ��𖳎�����
	//DirectX::XMStoreFloat4x4(&inv, matInv);
	//inv._41 = 0.0f; inv._42 = 0.0f; inv._43 = 0.0f;
	//matInv = DirectX::XMLoadFloat4x4(&inv);
	//matInv = // �t�s��̌v�Z
	//DirectX::XMMATRIX world = ;// ���[���h�s��ƃr���{�[�h�̍s����|�����킹������(�����s)
	//DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(world));
	//Sprite::SetWorld(mat[0]);
	//Sprite::SetTexture(m_pMarkerTex);
	//Sprite::Draw();

	DirectX::XMFLOAT4X4 mat[3];
	DirectX::XMStoreFloat4x4(&mat[0],
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
		));
	mat[1] = m_pCamera->GetViewMatrix();
	mat[2] = m_pCamera->GetProjectionMatrix();

	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();

//	DirectX::XMMATRIX mat1 = DirectX::XMMatrixTranslation(m_pos.x, 0.0f, m_pos.z);
	DirectX::XMMATRIX mat1 = DirectX::XMMatrixTranslation(m_pos.x, 0.0f, m_pos.z);
	mat1 = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f)) * mat1;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(mat1));
	Sprite::SetWorld(world);	// ���[���h�s��̐ݒ�
	Sprite::SetView(m_pCamera->GetViewMatrix());	// �r���[�s��̐ݒ�
	Sprite::SetProjection(m_pCamera->GetProjectionMatrix());	// �v���W�F�N�V�����s��̐ݒ�
	Sprite::SetSize(DirectX::XMFLOAT2(0.5f, 0.5f));	// �T�C�Y��ݒ�
	Sprite::SetTexture(m_pShadowTex);	// �e�N�X�`����ݒ�

	Sprite::Draw();

	// �G�t�F�N�g�̕`��
	m_pEffect->Draw(
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix());
}

void Player::SetCamera(CameraBase* pCamera)
{
	m_pCamera = pCamera;
}

DirectX::XMFLOAT3 Player::GetPos()
{
	return m_pos;
}