#include "Object.h"
#include "Geometory.h"
#include "Input.h"

//InputManager imanagerOB = InputManager();

DirectX::XMFLOAT3 objectMinBound = DirectX::XMFLOAT3(-0.1f, -0.5f, -0.1f);//�v���C���[�Ƃ̓����蔻��p
DirectX::XMFLOAT3 objectMaxBound = DirectX::XMFLOAT3(0.2f, 0.5f, -0.05f);

DirectX::XMFLOAT3 hobjectMinBound = DirectX::XMFLOAT3(-0.25f, -0.5f, -0.1f);//�߈˗p
DirectX::XMFLOAT3 hobjectMaxBound = DirectX::XMFLOAT3(0.25f, 0.5f, 0.35f);

DirectX::XMFLOAT3 cobjectMinBound = DirectX::XMFLOAT3(-0.3f, -0.5f, -0.3f);//�u���b�N���m�p
DirectX::XMFLOAT3 cobjectMaxBound = DirectX::XMFLOAT3(0.3f, 0.5f, 0.5f);



Object::Object()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, m_direction(0.0f, 0.0f, 0.0f)
	, m_rotationMatrix(DirectX::XMMatrixIdentity())
	, moveok(false)

	,m_isPossessed(true)
{
	m_pObjectModel = new Model;

	if (!m_pObjectModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}
	m_pObjectVS = new VertexShader();
	if (FAILED(m_pObjectVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pObjectModel->SetVertexShader(m_pObjectVS);





	SetBounds(minBound, maxBound);
	HSetBounds(hminBound, hmaxBound);
	CSetBounds(cminBound, cmaxBound);
	
}

Object::~Object()
{
	if (m_pObjectModel)
	{
		delete m_pObjectModel;
		m_pObjectModel = nullptr;
	}
	if (m_pObjectVS)
	{
		delete m_pObjectVS;
		m_pObjectVS = nullptr;
	}
}

void Object::Update()
{
	
		m_oldPos = m_pos;

	/*imanagerOB.addKeycode(0, 0, GAMEPAD_KEYTYPE::ThumbLL, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(1, 0, GAMEPAD_KEYTYPE::ThumbLR, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(2, 0, GAMEPAD_KEYTYPE::ThumbLU, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(3, 0, GAMEPAD_KEYTYPE::ThumbLD, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	imanagerOB.addKeycode(4, 0, GAMEPAD_KEYTYPE::Buttons, XINPUT_GAMEPAD_B);

	imanagerOB.inspect();*/

	float moveSpeed = 0.03f; // �ړ����x�̒���

	float rotationSpeed = 10.0f;

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

	if (moveok == true)
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
	}


	SetBounds(objectMinBound, objectMaxBound);  //�ŏ��l�ƍő�l���Z�b�g

	HSetBounds(hobjectMinBound, hobjectMaxBound);//�߈˗p�̓����蔻��
	CSetBounds(cobjectMinBound, cobjectMaxBound);//�u���b�N���m�̓����蔻��


}

void Object::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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
	
	m_pObjectVS->WriteBuffer(0, mat);    //�z��̐擪�A�h���X���w�肵�āA�܂Ƃ߂ĕϊ��s���n��
	m_pObjectModel->Draw();

}

//
void Object::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Object::GetminBounds() 
{
	return minBound;
}

DirectX::XMFLOAT3 Object::GetmaxBounds() 
{
	return maxBound;
}

DirectX::XMFLOAT3 Object::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//�߈˓����蔻��
void Object::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Object::HGetminBounds()
{
	return hminBound;
}

DirectX::XMFLOAT3 Object::HGetmaxBounds()
{
	return hmaxBound;
}

DirectX::XMFLOAT3 Object::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//�u���b�N���m
void Object::CSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	cminBound = CAdd(m_pos, min);
	cmaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Object::CGetminBounds()
{
	return cminBound;
}

DirectX::XMFLOAT3 Object::CGetmaxBounds()
{
	return cmaxBound;
}

DirectX::XMFLOAT3 Object::CAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

void Object::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
}
//



//�߈˔���p
void Object::Set()
{
	moveok = true;
}

void Object::SetF()
{
	moveok = false;
}

bool Object::SetR()
{
	return moveok;
}



//�u���b�N���m���Ԃ��������ɕԂ�

void Object::OBJPos()
{


	m_pos = m_oldPos;
}



void Object::Modelchg()
{
	if (m_pObjectModel->Load("Assets/Model/test_model/test_block.fbx", 0.05f, Model::Flip::XFlip));
}

void Object::Modelchg2()
{
	if (m_pObjectModel->Load("Assets/Model/Block/test_black_cube_tex_plus.fbx", 0.05f, Model::Flip::XFlip));
}
