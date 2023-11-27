#include "Player.h"
#include "Input.h"


//InputManager imanagerP = InputManager();



DirectX::XMFLOAT3 MinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);  //���E�̍ŏ��l
DirectX::XMFLOAT3 MaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);     //�ő�l

DirectX::XMFLOAT3 HMinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);  //���E�̍ŏ��l
DirectX::XMFLOAT3 HMaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);     //�ő�l


Player::Player()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	,ok (false)
{
	m_pModel = new Model;
	 //���f���̓ǂݍ��ݏ���
	if (!m_pModel->Load("Assets/Model/�����c�߂߂�/MokotaMememe.pmx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}
	
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pModel->SetVertexShader(m_pVS);

	
	minBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	maxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);

	hminBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	hmaxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);

	
}

Player::~Player()
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

void Player::Update()
{
	//�߈ˉ������ɜ߈˂������̈ʒu�Ƀv���C���[��߂�����
	if (ok==false)
	{
		m_oldPos = m_pos;
	}
	
	//m_pCamera->Update();
	//�Q�[���p�b�h�̑Ή�
	/*imanagerP.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerP.inspect();
*/
	float moveSpeed = 0.03f; // �ړ����x�̒���

	float rotationSpeed = 10.0f;

	// ���X�e�B�b�N��X����Y�������̓��͂��擾
	/*float leftStickX1 = static_cast<float>(imanagerP.getKey(0));
	float leftStickX2 = static_cast<float>(imanagerP.getKey(1));
	float leftStickZ1 = static_cast<float>(imanagerP.getKey(2));
	float leftStickZ2 = static_cast<float>(imanagerP.getKey(3));*/




	// �ړ������x�N�g�����v�Z
	//DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	// �ړ������x�N�g���𐳋K��
	//DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	//directionVector = DirectX::XMVector3Normalize(directionVector);
	//DirectX::XMFLOAT3 normalizedDirection;
	//DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	//// �ړ������x�N�g�������]�p�x���v�Z
	//float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//m_rotationY = rotationAngle;

	// �ʒu���X�V


	/*m_pos.x -= moveSpeed * moveDirection.x;
	m_pos.z -= moveSpeed * moveDirection.z;*/

	// ��]�s����X�V
	//m_rotationMatrix = DirectX::XMMatrixRotationY(m_rotationY);

	//if (moveDirection.x == 0.0f && moveDirection.z == 0.0f)
	//{
	//	// �Ō�Ɍ����Ă����������g�p
	//	m_rotationY = m_lastFacingDirection;
	//}
	//else
	//{
	//	// �ړ������x�N�g�������]�p�x���v�Z
	//	float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//	m_rotationY = rotationAngle;

	//	// �Ō�Ɍ������������X�V
	//	m_lastFacingDirection = m_rotationY;
	//}


	    if (IsKeyPress(VK_UP))
		{
			m_pos.z += moveSpeed;
		}
		if (IsKeyPress(VK_DOWN))
		{
			m_pos.z -= moveSpeed;
		}
		if (IsKeyPress(VK_RIGHT))
		{
			m_pos.x += moveSpeed;
		}
		if (IsKeyPress(VK_LEFT))
		{
			m_pos.x -= moveSpeed;
		}


	SetBounds(MinBound, MaxBound);  //�ŏ��l�ƍő�l���Z�b�g
	HSetBounds(HMinBound, HMaxBound);
	
}

void Player::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	// ���[���h�s��̌v�Z�ɉ�]��ǉ�
	DirectX::XMMATRIX MT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	//DirectX::XMMATRIX MR = DirectX::XMMatrixRotationY(m_rotationY); // Y����]
	DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX world = MS * MT;//MR* MT; // ��]��K�p


	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = viewMatrix;
	mat[2] = projectionMatrix;

	 //�s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();
}

void Player::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Player::GetminBox()
{
	return minBound;
}

DirectX::XMFLOAT3 Player::GetmaxBox()
{
	return maxBound;
}


DirectX::XMFLOAT3 Player::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//pos�ɍŏ��l�A�ő�l�𑫂��ē����蔻������炷
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}



//�߈˓����蔻��
void Player::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Player::HGetminBox()
{
	return hminBound;
}

DirectX::XMFLOAT3 Player::HGetmaxBox()
{
	return hmaxBound;
}


DirectX::XMFLOAT3 Player::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	//pos�ɍŏ��l�A�ő�l�𑫂��ē����蔻������炷
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//__


//�u���b�N�ƃv���C���[�Փˎ��A�v���C���[�̈ʒu��Ԃ�
void Player::PlayerPos()
{
	m_pos = m_oldPos;
}

//�߈ˎ��ɔ�΂�
void Player::HPlayerPos()
{
	m_pos.y = 100.0f;
}

//���Z�b�g�p�i�������j
void Player::RPlayerPos()
{
	m_pos.z = 0.0f;
	m_pos.x = 0.0f;

}

//
bool Player::Set()
{
	return true;
}
void Player::SetOk()
{
	ok = true;
}
void Player::SetNOk()
{
	ok = false;
}