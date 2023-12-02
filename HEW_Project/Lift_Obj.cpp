#include "Lift_Obj.hpp"
#include "Geometory.h"


// プレイヤーとの当たり判定用
DirectX::XMFLOAT3 liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
DirectX::XMFLOAT3 liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

// ブロック同士の当たり判定用
DirectX::XMFLOAT3 c_liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
DirectX::XMFLOAT3 c_liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

Lift_Obj::Lift_Obj()
	: m_pos(0.0f, 0.0f, 0.0f), m_oldPos(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_rotY(0.0f), m_lastRotY(0.0f)
	, m_direction(0.0f, 0.0f, 1.0f)
	, m_rotMat(DirectX::XMMatrixIdentity())
	, moveOk(false)
	, m_pModel(nullptr), m_pVS(nullptr)
{
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/Block/test_black_cube.fbx", 0.05f, Model::Flip::XFlip))
	{
		MessageBox(nullptr, "Liftモデルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pVS = new VertexShader();
	if (!m_pVS->Load("Assets/Shader/VS_Model.cso"))
	{
		MessageBox(nullptr, "Lift頂点シェーダーの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pModel->SetVertexShader(m_pVS);

	// それぞれの当たり判定の大きさを設定
	SetBounds(liftobj_MinBound, liftobj_MaxBound);
	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);

}

Lift_Obj::~Lift_Obj()
{
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

void Lift_Obj::Update()
{
}

void Lift_Obj::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
}

void Lift_Obj::SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
}

DirectX::XMFLOAT3 Lift_Obj::GetminBounds()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Lift_Obj::GetmaxBounds()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Lift_Obj::Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	return DirectX::XMFLOAT3();
}

void Lift_Obj::CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
}

DirectX::XMFLOAT3 Lift_Obj::CGetminBounds()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Lift_Obj::CGetmaxBounds()
{
	return DirectX::XMFLOAT3();
}

DirectX::XMFLOAT3 Lift_Obj::CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	return DirectX::XMFLOAT3();
}

void Lift_Obj::LiftObj_SetPos()
{
}
