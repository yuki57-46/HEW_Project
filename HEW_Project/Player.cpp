#include "Player.h"
#include "Input.h"
#include"Gamepad.h"
#include <chrono>

InputManager imanagerP = InputManager();

DirectX::XMFLOAT3 MinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);  //���E�̍ŏ��l
DirectX::XMFLOAT3 MaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);     //�ő�l

DirectX::XMFLOAT3 HMinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.3f);  //���E�̍ŏ��l
DirectX::XMFLOAT3 HMaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);     //�ő�l

std::chrono::steady_clock::time_point lastSoundPlayTimePly;
const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(2000);//�Đ����ԎO�b�̎�

Player::Player()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, m_pSVSEPly(nullptr)//�X�s�[�J
	, m_pSDSEPly(nullptr)//�T�E���h�f�[�^
	, ok (false)
	, m_pVS(nullptr)
	, m_anime_Levitation(NULL)
	, m_anime_possession(NULL)
{
	m_pModel = new Model;
	 //���f���̓ǂݍ��ݏ���
	if (!m_pModel->Load("Assets/Model/Player/kuroko.fbx",0.5f, Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}
	
	// ���f����ShaderList����VS,PS��ǂݍ���
	m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_ANIME));
	m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_TOON));

	m_pVS = new VertexShader();

	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}

//	m_pModel->SetVertexShader(m_pVS);

	m_anime_Levitation = m_pModel->AddAnimation("Assets/Animation/kuroko_huyu.fbx");	//	�t�@�C���p�X������
	if (FAILED(m_pModel->AddAnimation("Assets/Animation/kuroko_huyu.fbx")))
	{
		MessageBox(nullptr, "anime", "Error", MB_OK);
	}
	m_anime_possession = m_pModel->AddAnimation("Assets/Animation/kuroko_hyoui.fbx");

	
	minBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	maxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);

	hminBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	hmaxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);

	m_pSDSEPly= LoadSound("Assets/Sound/SE/yuurei idouonn_Arai_1.wav");
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

void Player::Update(float tick)
{

	//�߈ˉ������ɜ߈˂������̈ʒu�Ƀv���C���[��߂�����
	if (ok==false)
	{
		m_oldPos = m_pos;
	}
	float moveSpeed = 0.03f; // �ړ����x�̒���

	float rotationSpeed = 1.0f;
	
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTimePly);

	m_pModel->Step(tick);

	if (ok == false)
	{
		m_pModel->Play(m_anime_Levitation, true);	// ���V�A�j���[�V����(�펞)
	}
	//else
	//{
	//	m_pModel->Play(m_anime_possession, false);	// ���[�v�����A�j���[�V����
	//}

	//m_pCamera->Update();
	//�Q�[���p�b�h�̑Ή�
	imanagerP.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerP.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);
	imanagerP.addKeycode(5, 0, GAMEPAD_KEYTYPE::LTrigger, XINPUT_GAMEPAD_LEFT_SHOULDER);
	imanagerP.addKeycode(6, 0, GAMEPAD_KEYTYPE::RTrigger, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	imanagerP.inspect();

	// ���X�e�B�b�N��X����Y�������̓��͂��擾
	float leftStickX1 = static_cast<float>(imanagerP.getKey(0));
	float leftStickX2 = static_cast<float>(imanagerP.getKey(1));
	float leftStickZ1 = static_cast<float>(imanagerP.getKey(2));
	float leftStickZ2 = static_cast<float>(imanagerP.getKey(3));



	// �ړ������x�N�g�����v�Z
	DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	 //�ړ������x�N�g���𐳋K��
	DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	directionVector = DirectX::XMVector3Normalize(directionVector);
	DirectX::XMFLOAT3 normalizedDirection;
	DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	// �ړ������x�N�g�������]�p�x���v�Z
	float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	m_rotationY = rotationAngle;

	// �ʒu���X�V
	m_pos.x -= moveSpeed * moveDirection.x;
	m_pos.z -= moveSpeed * moveDirection.z;


	if((imanagerP.getKey(0) & 0b011)|| (imanagerP.getKey(1) & 0b011)||
		(imanagerP.getKey(2) & 0b011)|| (imanagerP.getKey(3) & 0b011))
	{
		if (elapsedTime >= soundInterval)
		{
			m_pSVSEPly = PlaySound(m_pSDSEPly);

			// �Ō�̃T�E���h�Đ����Ԃ��X�V
			lastSoundPlayTimePly = currentTime;
		}
	}

	// ��]�s����X�V
	m_rotationMatrix = DirectX::XMMatrixRotationY(m_rotationY);

	if (imanagerP.getKey(5) & 0b011)
	{
		m_pos.y -= moveSpeed * 1.0f;
	}
	if (imanagerP.getKey(6) & 0b011)
	{
		m_pos.y += moveSpeed * 1.0f;
	}


	if (moveDirection.x == 0.0f && moveDirection.z == 0.0f)
	{
		// �Ō�Ɍ����Ă����������g�p
		m_rotationY = m_lastFacingDirection;
	}
	else
	{
		// �ړ������x�N�g�������]�p�x���v�Z
		float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
		m_rotationY = rotationAngle;
		// �Ō�Ɍ������������X�V
		m_lastFacingDirection = m_rotationY;
	}

	if (ok == false)
	{
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
		if (IsKeyPress('U'))
		{
			m_pos.y -= moveSpeed;
		}
		if (IsKeyPress('I'))
		{
			m_pos.y += moveSpeed;
		}
	}

		SetBounds(MinBound, MaxBound);  //�ŏ��l�ƍő�l���Z�b�g
		HSetBounds(HMinBound, HMaxBound);


		if (m_pos.x>=7.0f||m_pos.x<=-7.0f
			||m_pos.z>=7.0f|| m_pos.z <= -5.0f
			||m_pos.y >= 6.0f )
		{
			PlayerPos();
		}
}

void Player::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	// ���[���h�s��̌v�Z�ɉ�]��ǉ�
	DirectX::XMMATRIX MT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	//DirectX::XMMATRIX MR = DirectX::XMMatrixRotationY(m_rotationY); // Y����]
	DirectX::XMMATRIX MS = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX world = MS  * MT;//MR* MT; // ��]��K�p


	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);
	mat[1] = viewMatrix;
	mat[2] = projectionMatrix;

	 //�s����V�F�[�_�[�֐ݒ�
	m_pVS->WriteBuffer(0, mat);
//	m_pModel->Draw();

	ShaderList::SetWVP(mat);	// �]�u�ς݂̕ϊ��s��

	m_pModel->Draw(nullptr, [this](int index)	// �����_��
	{
		const Model::Mesh* pMesh = m_pModel->GetMesh(index);
		const Model::Material* pMaterial = m_pModel->GetMaterial(pMesh->materialID);
		ShaderList::SetMaterial(*pMaterial);

		DirectX::XMFLOAT4X4 bones[200];
		for (int i = 0; i < pMesh->bones.size() && i < 200; ++i)
		{
			// ���̌v�Z�̓Q�[������[�u�X�L�����b�V���̎d�g�݁v���Q�l�ɂȂ�
			DirectX::XMStoreFloat4x4(&bones[i], DirectX::XMMatrixTranspose(
				pMesh->bones[i].invOffset *
				m_pModel->GetBone(pMesh->bones[i].index)
			));
		}
		ShaderList::SetBones(bones);
	});
#ifdef _DEBUG
//	m_pModel->DrawBone();

#endif // DEBUG
		
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
	m_pos.y = -100.0f;
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

float Player::GetPosX()
{
	return m_pos.x;
}

float Player::GetPosY()
{
	return m_pos.y;
}

float Player::GetPosZ()
{
	return m_pos.z;
}

/**
 * @brief �A�j���[�V�����̐ݒ�
 * @param[in] �Ȃ�
 * @return �Ȃ�
 */
void Player::SetAnime()
{
	// �����A�j���[�V�����̃Z�b�g

}

/**
 * @brief �A�j���[�V�����̐ݒ�
 * @param[in] �Ȃ�
 * @return �Ȃ�
 */
void Player::SetAnime2()
{
	m_pModel->Play(m_anime_possession, false);	// �߈˂̕`��
}
