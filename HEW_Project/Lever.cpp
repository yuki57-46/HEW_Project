﻿#include "Lever.h"
#include "Geometory.h"
#include "Input.h"
#include <chrono>

//minbound maxboundをメンバ変数に
//create部分にスケールと当たり判定をかけ合わせる処理を追加　yは追加で計算必要



Lever::Lever()
	: m_pos(0.0f, 0.0f, 0.0f)
	, m_scale(0.0f, 0.0f, 0.0f)
	, m_oldPos(0.0f, 0.0f, 0.0f)
	, LeverMinBound(-0.5f, -0.5f, -0.5f)//当たり判定用
	, LeverMaxBound(0.5f, 0.5f, 0.5f)
	, hLeverMinBound(-0.5f, -0.5f, -0.5f)
	, hLeverMaxBound(0.5f, 0.5f, 0.5f)
	
{
	m_pLeverModel = new Model;


	if (!m_pLeverModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip)) {
		MessageBox(NULL, "モデルの読み込みエラー‗レバー", "Error", MB_OK);
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

	

	SetBounds(LeverMinBound, LeverMaxBound);  //最小値と最大値をセット
	HSetBounds(hLeverMinBound, hLeverMaxBound);//憑依用の当たり判定
	

}
void Lever::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	DirectX::XMMATRIX MoT = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	DirectX::XMMATRIX MoS = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	DirectX::XMMATRIX world = MoS * MoT;
	//world = [ワールド行列の設定];
	world = DirectX::XMMatrixTranspose(world);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 与えられた viewMatrix を使う
	mat[2] = projectionMatrix; // 与えられた projectionMatrix を使う

	m_pLeverVS->WriteBuffer(0, mat);    //配列の先頭アドレスを指定して、まとめて変換行列を渡す
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


//憑依当たり判定
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





void Lever::ModelchgUp()
{
	if (m_pLeverModel->Load("Assets/Model/test_model/test_block.fbx", Model::Flip::XFlip));
}

void Lever::ModelchgDown()
{
	if (m_pLeverModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip));
}


void Lever::ModelchgNormal()
{
	if (m_pLeverModel->Load("Assets/Model/Block/BoxS.fbx", Model::Flip::XFlip));
}
