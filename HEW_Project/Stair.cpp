#include "Stair.h"
#include "Geometory.h"
#include "Input.h"
#include <chrono>

//minbound maxbound�������o�ϐ���
//create�����ɃX�P�[���Ɠ����蔻����������킹�鏈����ǉ��@y�͒ǉ��Ōv�Z�K�v


//InputManager imanagerOB = InputManager();

//DirectX::XMFLOAT3 StairMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//�v���C���[�Ƃ̓����蔻��p
//DirectX::XMFLOAT3 StairMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//DirectX::XMFLOAT3 hStairMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//�߈˗p
//DirectX::XMFLOAT3 hStairMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//
//DirectX::XMFLOAT3 cStairMinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);//�u���b�N���m�p
//DirectX::XMFLOAT3 cStairMaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);
//std::chrono::steady_clock::time_point lastSoundPlayTime1;
//const std::chrono::milliseconds soundInterval = std::chrono::milliseconds(3000);//�Đ����ԎO�b�̎�
Stair::Stair()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_mmovespeed(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_jmp(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)
	, StairMinBound(-0.5f, -0.5f, -0.5f)//�����蔻��p
	, StairMaxBound(0.5f, 0.5f, 0.5f)
	, hStairMinBound(-0.5f, -0.5f, -0.5f)
	, hStairMaxBound(0.5f, 0.5f, 0.5f)
	, cStairMinBound(-0.5f, -0.5f, -0.5f)
	, cStairMaxBound(0.5f, 0.5f, 0.5f)
	, ok(true)
	, gravity(false)
	, colgravity(true)
	, xz(false)
	, StairTop(false)
	, m_pSVSEBlk(nullptr)
	, m_pSDSEBlk(nullptr)
{
	m_pStairModel = new Model;

	/*if (!m_pStairModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}*/
	/*if (!m_pStairModel->Load("Assets/Model/Block/Slope.fbx",0.1,  Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}*/
	if (!m_pStairModel->Load("Assets/Model/Block/Slope.fbx", Model::Flip::XFlip))/*BoxS.fbx*/ {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}
	ExtractSlopeVertexCoordinates(*m_pStairModel);
	m_pStairVS = new VertexShader();
	if (FAILED(m_pStairVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pStairModel->SetVertexShader(m_pStairVS);


	SetBounds(minBound, maxBound);
	HSetBounds(hminBound, hmaxBound);
	CSetBounds(cminBound, cmaxBound);

	m_pSDSEBlk = LoadSound("Assets/Sound/SE/Blockgaugokuoto_Oobayashi.wav");

	v0.x = 1.5f;
	v0.y = 0.0f;
	v0.z = -0.5f;
	v1.x = 2.5f;
	v1.y = 0.0f;
	v1.z = -0.5f;
	v2.x = 2.5f;
	v2.y = 1.0f;
	v2.z = -0.5f;

	point.x = -0.5;
	point.y = 0.0f;
	point.z = 3.0f;

	points = {
	{minBound.x, minBound.y, minBound.z},  //����
	{minBound.x, minBound.y, maxBound.z},
	{minBound.x + 0.5f, minBound.y, minBound.z},
	{minBound.x + 0.5f, minBound.y, maxBound.z},
	{maxBound.x, minBound.y, minBound.z},  //�E��
	{maxBound.x, minBound.y, maxBound.z},
	{maxBound.x, minBound.y + 0.5f, minBound.z},
	{maxBound.x, minBound.y + 0.5f, maxBound.z},
	{maxBound.x, maxBound.y, minBound.z},  //�E��
	{maxBound.x, maxBound.y, maxBound.z},
	{minBound.x + 0.75f, minBound.y + 0.75f, minBound.z},
	{minBound.x + 0.75f, minBound.y + 0.75f, maxBound.z},
	{minBound.x + 0.5f, minBound.y + 0.5f, minBound.z},
	{minBound.x + 0.5f, minBound.y + 0.5f, maxBound.z},
	{minBound.x + 0.25f, minBound.y + 0.25f, minBound.z},
	{minBound.x + 0.25f, minBound.y + 0.25f, maxBound.z},
	{minBound.x + 0.15f, minBound.y + 0.15f, minBound.z},
	{minBound.x + 0.15f, minBound.y + 0.15f, maxBound.z},
	};
}

Stair::~Stair()
{
	if (m_pStairModel)
	{
		delete m_pStairModel;
		m_pStairModel = nullptr;
	}
	if (m_pStairVS)
	{
		delete m_pStairVS;
		m_pStairVS = nullptr;
	}
	//m_pSVSEBlk->Stop();
}

void Stair::Update()
{

	m_oldPos = m_pos;
	if (colgravity == true)
	{
		m_pos.y -= 0.05f;
	}
	float moveSpeed = 0.03f; // �ړ����x�̒���
	float rotationSpeed = 10.0f;

	/*imanagerOB.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerOB.inspect();*/



	//// ���X�e�B�b�N��X����Y�������̓��͂��擾
	//float leftStickX1 = static_cast<float>(imanagerOB.getKey(0));
	//float leftStickX2 = static_cast<float>(imanagerOB.getKey(1));
	//float leftStickZ1 = static_cast<float>(imanagerOB.getKey(2));
	//float leftStickZ2 = static_cast<float>(imanagerOB.getKey(3));



	// �ړ������x�N�g�����v�Z
	//DirectX::XMFLOAT3 moveDirection = DirectX::XMFLOAT3(leftStickX1 - leftStickX2, 0.0f, leftStickZ1 - leftStickZ2);

	//// �ړ������x�N�g���𐳋K���i������1�ɂȂ�悤�Ɂj
	//DirectX::XMVECTOR directionVector = DirectX::XMVectorSet(moveDirection.x, 0.0f, moveDirection.z, 0.0f);
	//directionVector = DirectX::XMVector3Normalize(directionVector);
	//DirectX::XMFLOAT3 normalizedDirection;
	//DirectX::XMStoreFloat3(&normalizedDirection, directionVector);

	//// �ړ������x�N�g�������]�p�x���v�Z
	//float rotationAngle = atan2(normalizedDirection.x, normalizedDirection.z);
	//m_rotationY = rotationAngle;

	//if (moveok == true)//�߈ˎ�
	//{
	//	m_pos.x -= moveSpeed * moveDirection.x;
	//	m_pos.z -= moveSpeed * moveDirection.z;
	//}

	//auto currentTime = std::chrono::steady_clock::now();
//	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSoundPlayTime);

	//m_jmp = m_pos;

	if (m_pos.y <= 0.0f)
	{
		//ok = false;
		gravity = false;
	}
	if (moveok == true)
	{
		if (IsKeyPress(VK_UP))
		{
			m_pos.z += moveSpeed;

			if (m_pos.y <= 0.0f)
			{
				//if (elapsedTime >= soundInterval)
				//{
				//	m_pSVSEBlk = PlaySound(m_pSDSEBlk);

				//	// �Ō�̃T�E���h�Đ����Ԃ��X�V
				//	lastSoundPlayTime = currentTime;
				////}
			}
			xz = true;
		}
		else if (IsKeyPress(VK_DOWN))
		{
			m_pos.z -= moveSpeed;
			if (m_pos.y <= 0.0f)
			{
				//if (elapsedTime >= soundInterval)
				//{
				//	m_pSVSEBlk = PlaySound(m_pSDSEBlk);

				//	// �Ō�̃T�E���h�Đ����Ԃ��X�V
				//	lastSoundPlayTime = currentTime;
				//}
			}
			xz = true;
		}
		else if (IsKeyPress(VK_RIGHT))
		{
			m_pos.x += moveSpeed;
			if (m_pos.y <= 0.0f)
			{
				//if (elapsedTime >= soundInterval)
				//{
				//	m_pSVSEBlk = PlaySound(m_pSDSEBlk);

				//	// �Ō�̃T�E���h�Đ����Ԃ��X�V
				//	lastSoundPlayTime = currentTime;
				//}
			}
			xz = true;
		}
		else if (IsKeyPress(VK_LEFT))
		{
			m_pos.x -= moveSpeed;
			if (m_pos.y <= 0.0f)
			{
				//if (elapsedTime >= soundInterval)
				//{
				//	m_pSVSEBlk = PlaySound(m_pSDSEBlk);

				//	// �Ō�̃T�E���h�Đ����Ԃ��X�V
				//	lastSoundPlayTime = currentTime;
				//}
			}
			xz = true;
		}
		else
		{
			xz = false;
		}

		if (IsKeyPress('U'))
		{
			m_pos.y -= moveSpeed;
		}
		if (IsKeyPress('I'))
		{
			m_pos.y += moveSpeed;
		}




		if (ok == false)
		{

			if (IsKeyPress(VK_SPACE))
			{
				frame -= moveSpeed * 0.01;
				// �X�y�[�X�L�[�������ꂽ��㏸�����s.�Q�[�W������
			   //m_pos.y += 0.07f;
				m_pos.y += frame * 0.003f;

				if (m_pos.y > 2.5f)
				{
					m_pos.y = m_oldPos.y;
				}
			}
			if (frame <= 0 || !(IsKeyPress(VK_SPACE)))
			{
				m_pos.y -= 0.1f;
				gravity = true;
			}
			if (m_pos.y <= 0.0f&&frame <= 0)
			{
				frame = 50;
			}

		}
		else if (ok == true)
		{
			m_pos.y -= 0.05f;
			gravity = true;
			//m_jmp = m_pos;
		}
	}

	SetBounds(StairMinBound, StairMaxBound);  //�ŏ��l�ƍő�l���Z�b�g
	HSetBounds(hStairMinBound, hStairMaxBound);//�߈˗p�̓����蔻��
	CSetBounds(cStairMinBound, cStairMaxBound);//�u���b�N���m�̓����蔻��


	if (m_pos.x >= 7.0f || m_pos.x <= -7.0f
		|| m_pos.z >= 7.0f || m_pos.z <= -5.0f
		|| m_pos.y >= 7.0f)
	{

		OBJPos();
	}

	/*if (m_pos.y <= 0.0f)
	{
		ok = false;
		gravity = false;
	}*/

	if (m_pos.y <= 0.0f)
	{
		SetF1();
		OBJPosy();
		//m_pos.y = 0.0f;
		gravity = false;
	}

	//points = {
	//{-0.5f, 0.0f, 3.0f},
	//{ 0.0f, 0.0f, 3.0f},
	//{ 0.5f, 0.0f, 3.0f},
	//{ 0.5f, 0.5f, 3.0f},
	//{ 0.5f, 0.8f, 3.0f},
	//{ 0.0f, 0.5f, 3.0f},
	//// Add more points as needed
	//};
	points = {
	{minBound.x, minBound.y, minBound.z},  //����
	{minBound.x, minBound.y, maxBound.z},
	{minBound.x + 0.5f, minBound.y, minBound.z},
	{minBound.x + 0.5f, minBound.y, maxBound.z},
	{maxBound.x, minBound.y, minBound.z},  //�E��
	{maxBound.x, minBound.y, maxBound.z},  
	{maxBound.x, minBound.y + 0.5f, minBound.z},
	{maxBound.x, minBound.y + 0.5f, maxBound.z},
	{maxBound.x, maxBound.y, minBound.z},  //�E��
	{maxBound.x, maxBound.y, maxBound.z}, 
	{minBound.x + 0.75f, minBound.y + 0.75f, minBound.z},
	{minBound.x + 0.75f, minBound.y + 0.75f, maxBound.z},
	{minBound.x + 0.5f, minBound.y + 0.5f, minBound.z},  
	{minBound.x + 0.5f, minBound.y + 0.5f, maxBound.z},  
	{minBound.x + 0.25f, minBound.y + 0.25f, minBound.z},
	{minBound.x + 0.25f, minBound.y + 0.25f, maxBound.z},
	{minBound.x + 0.15f, minBound.y + 0.15f, minBound.z},
	{minBound.x + 0.15f, minBound.y + 0.15f, maxBound.z},
	};

}
void Stair::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	DirectX::XMMATRIX world = MoS * MoT;
	//world = [���[���h�s��̐ݒ�];
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // �^����ꂽ viewMatrix ���g��
	mat[2] = projectionMatrix; // �^����ꂽ projectionMatrix ���g��

	m_pStairVS->WriteBuffer(0, mat);    //�z��̐擪�A�h���X���w�肵�āA�܂Ƃ߂ĕϊ��s���n��
	m_pStairModel->Draw();

}


void Stair::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Stair::GetminBounds()
{
	return minBound;
}

DirectX::XMFLOAT3 Stair::GetmaxBounds()
{
	return maxBound;
}

DirectX::XMFLOAT3 Stair::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//�߈˓����蔻��
void Stair::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Stair::HGetminBounds()
{
	return hminBound;
}

DirectX::XMFLOAT3 Stair::HGetmaxBounds()
{
	return hmaxBound;
}

DirectX::XMFLOAT3 Stair::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//�u���b�N���m
void Stair::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Stair::CGetminBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 Stair::CGetmaxBounds()
{
	return cmaxBound;
}

DirectX::XMFLOAT3 Stair::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

DirectX::XMFLOAT3 Stair::GetStairMaxBounds()
{
	return cStairMaxBound;
}

DirectX::XMFLOAT3 Stair::GetPos()
{
	return m_pos;
}

void Stair::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;


	//
	StairMinBound.x *= m_scale.x;
	StairMinBound.y *= m_scale.y;
	StairMinBound.z *= m_scale.z;
	StairMaxBound.x *= m_scale.x;
	StairMaxBound.y *= m_scale.y;
	StairMaxBound.z *= m_scale.z;

	if (StairMinBound.y < 0)
	{
		a = StairMinBound.y *= -1;
		StairMaxBound.y += a;

		StairMinBound.y = 0;
	}
	SetBounds(StairMinBound, StairMaxBound);

	cStairMinBound.x *= m_scale.x;
	cStairMinBound.y *= m_scale.y;
	cStairMinBound.z *= m_scale.z;
	cStairMaxBound.x *= m_scale.x;
	cStairMaxBound.y *= m_scale.y;
	cStairMaxBound.z *= m_scale.z;

	//���ꂪ�Ȃ���y���̓����蔻�肨�������Ȃ�

	if (cStairMinBound.y < 0)
	{
		a = cStairMinBound.y *= -1;
		cStairMaxBound.y += a;

		cStairMinBound.y = 0;
	}

	CSetBounds(cStairMinBound, cStairMaxBound);


	hStairMinBound.x *= m_scale.x;
	hStairMinBound.y *= m_scale.y;
	hStairMinBound.z *= m_scale.z;
	hStairMaxBound.x *= m_scale.x;
	hStairMaxBound.y *= m_scale.y;
	hStairMaxBound.z *= m_scale.z;

	//if (cStairMinBound.y < 0)
	//{
	//	a = cStairMinBound.y *= -1;
	//	cStairMaxBound.y += a;

	//	cStairMinBound.y = 0;
	//}
	HSetBounds(hStairMinBound, hStairMaxBound);

	m_mmovespeed = m_pos;

}



//�߈˔���p
void Stair::Set()
{
	moveok = true;
}

void Stair::SetF()
{
	moveok = false;
}

bool Stair::SetR()
{
	return moveok;
}



//�u���b�N���m���Ԃ��������ɕԂ�

void Stair::OBJPos()
{
	m_pos = m_oldPos;
}

void Stair::OBJPosy()
{
	m_pos.y = m_oldPos.y;
}

void Stair::Modelchg()
{
	if (m_pStairModel->Load("Assets/Model/test_model/test_block.fbx", Model::Flip::XFlip));
}

void Stair::Modelchg2()
{
	if (m_pStairModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip));
}

void Stair::Set1()
{
	ok = true;
}

void Stair::SetF1()
{
	ok = false;
}

bool Stair::SetR1()
{
	return ok;
}

void Stair::SetColgravity()
{
	if (colgravity == true)
	{
		colgravity = false;
	}
	else/* if (colgravity == false)*/
	{
		colgravity = true;
	}
}

void Stair::SetStairTop()
{
	if (StairTop == true)
	{
		StairTop = false;
	}
	else/* if (colgravity == false)*/
	{
		StairTop = true;
	}
}

void Stair::MoveStair(float y)
{
	m_pos.y = y;
}


bool Stair::IsGravity()
{
	return gravity;
}


bool Stair::IsXZ()
{
	return xz;
}

bool Stair::IsStairTop()
{
	return StairTop;
}

void Stair::ExtractSlopeVertexCoordinates(Model& slopeModel)
{
	const Model::Mesh* slopeMesh = slopeModel.GetMesh(0); // Slope.fbx��1�̃��b�V�����������Ȃ��Ɖ���

	if (slopeMesh) {
		const std::vector<Model::Vertex>& vertices = slopeMesh->vertices;

		// ���_���W�� 'vertices' �x�N�^�[����A�N�Z�X�ł��܂�
		for (const auto& vertex : vertices) {
			DirectX::XMFLOAT3 position = vertex.pos;
			// ���_���W (position.x, position.y, position.z) ���g���ĉ������s���܂�
		}
	}
}

