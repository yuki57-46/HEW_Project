#include "Lever.h"
#include "Geometory.h"
#include "Input.h"
#include <chrono>

//minbound maxbound�������o�ϐ���
//create�����ɃX�P�[���Ɠ����蔻����������킹�鏈����ǉ��@y�͒ǉ��Ōv�Z�K�v



Lever::Lever()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, LeverMinBound(-0.5f, -0.5f, -0.5f)//�����蔻��p
	, LeverMaxBound(0.5f, 0.5f, 0.5f)
	, hLeverMinBound(-0.5f, -0.5f, -0.5f)
	, hLeverMaxBound(0.5f, 0.5f, 0.5f)
	
{
	m_pLeverModel = new Model;


	if (!m_pLeverModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "���f���̓ǂݍ��݃G���[", "Error", MB_OK);
	}
	m_pLeverVS = new VertexShader();
	if (FAILED(m_pLeverVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "VS_Model.cso", "Error", MB_OK);
	}
	m_pLeverModel->SetVertexShader(m_pLeverVS);


	SetBounds(minBound, maxBound);
	HSetBounds(hminBound, hmaxBound);


}

Lever::~Lever()
{
	if (m_pLeverModel)
	{
		delete m_pLeverModel;
		m_pLeverModel = nullptr;
	}
	if (m_pLeverVS)
	{
		delete m_pLeverVS;
		m_pLeverVS = nullptr;
	}
	
}

void Lever::Update()
{

	

	SetBounds(LeverMinBound, LeverMaxBound);  //�ŏ��l�ƍő�l���Z�b�g
	HSetBounds(hLeverMinBound, hLeverMaxBound);//�߈˗p�̓����蔻��
	

}
void Lever::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
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

	m_pLeverVS->WriteBuffer(0, mat);    //�z��̐擪�A�h���X���w�肵�āA�܂Ƃ߂ĕϊ��s���n��
	m_pLeverModel->Draw();

}


void Lever::SetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Lever::GetminBounds()
{
	return minBound;
}

DirectX::XMFLOAT3 Lever::GetmaxBounds()
{
	return maxBound;
}

DirectX::XMFLOAT3 Lever::Add(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_


//�߈˓����蔻��
void Lever::HSetBounds(const DirectX::XMFLOAT3 & min, const DirectX::XMFLOAT3 & max)
{
	hminBound = HAdd(m_pos, min);
	hmaxBound = HAdd(m_pos, max);
}

DirectX::XMFLOAT3 Lever::HGetminBounds()
{
	return hminBound;
}

DirectX::XMFLOAT3 Lever::HGetmaxBounds()
{
	return hmaxBound;
}

DirectX::XMFLOAT3 Lever::HAdd(const DirectX::XMFLOAT3 & a, const DirectX::XMFLOAT3 & b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
//_



void Lever::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;


	//
	LeverMinBound.x *= m_scale.x;
	LeverMinBound.y *= m_scale.y;
	LeverMinBound.z *= m_scale.z;
	LeverMaxBound.x *= m_scale.x;
	LeverMaxBound.y *= m_scale.y;
	LeverMaxBound.z *= m_scale.z;

	SetBounds(LeverMinBound, LeverMaxBound);




	hLeverMinBound.x *= m_scale.x;
	hLeverMinBound.y *= m_scale.y;
	hLeverMinBound.z *= m_scale.z;
	hLeverMaxBound.x *= m_scale.x;
	hLeverMaxBound.y *= m_scale.y;
	hLeverMaxBound.z *= m_scale.z;

	HSetBounds(hLeverMinBound, hLeverMaxBound);

	

}




//�u���b�N���m���Ԃ��������ɕԂ�



void Lever::Modelchg()
{
	if (m_pLeverModel->Load("Assets/Model/test_model/test_block.fbx", Model::Flip::XFlip));
}

void Lever::Modelchg2()
{
	if (m_pLeverModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip));
}


