﻿#include "Lift_Obj.hpp"
#include "Geometory.h"
#include "Safe_Delete.hpp"
#include "GameObject.h"
#include "Lever.h"


// プレイヤーとの当たり判定用
DirectX::XMFLOAT3 liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
DirectX::XMFLOAT3 liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

// ブロック同士の当たり判定用
DirectX::XMFLOAT3 c_liftobj_MinBound = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
DirectX::XMFLOAT3 c_liftobj_MaxBound = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);


Lever* g_pLever;

Lift_Obj::Lift_Obj()
	: m_pos(0.0f, 0.0f, 0.0f), m_oldPos(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
	, m_rotY(0.0f), m_lastRotY(0.0f)
	, m_direction(0.0f, 0.0f, 1.0f)
	, m_rotMat(DirectX::XMMatrixIdentity())
	, moveOk(false)
	, m_pModel(nullptr), m_pVS(nullptr)
	, m_heightPosY(0.0f), m_lowPosY(0.0f)
	, m_moveSpeed(0.0f)
	, m_RiseFlag(false)
{
	m_pModel = new Model();
	if (!m_pModel->Load("Assets/Model/Block/test_black_cube.fbx", 0.05f, Model::Flip::XFlip))
	{
		MessageBox(nullptr, "Liftモデルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pVS = new VertexShader();
	if (FAILED(m_pVS->Load("Assets/Shader/VS_Model.cso")))
	{
		MessageBox(nullptr, "Lift頂点シェーダーの読み込みに失敗しました", "エラー", MB_OK | MB_ICONWARNING);
	}
	m_pModel->SetVertexShader(m_pVS);

	minBound = DirectX::XMFLOAT3(-0.15f, -0.5f, -0.2f);
	maxBound = DirectX::XMFLOAT3(0.2f, 0.5f, 0.4f);
	// それぞれの当たり判定の大きさを設定
	//SetBounds(liftobj_MinBound, liftobj_MaxBound);
	SetBounds(minBound, maxBound);
	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);

}

Lift_Obj::~Lift_Obj()
{
	SafeDelete(m_pModel);
	SafeDelete(m_pVS);
}

void Lift_Obj::Update(bool LeverFlg)
{
	m_oldPos = m_pos; // 前の位置を保存

	// レバーが使用されている場合
	if (LeverFlg == true)
	{
		// レバーがオンの状態
		if (E_LiftState::Lift_UP)	// リフトが上に上がる処理
		{
			if (m_RiseFlag == false) // `false`になってたら
			{
				m_RiseFlag = true; // 上昇フラグを`true`に設定
			}
			// 上昇フラグが`true`の場合
			if (m_RiseFlag)
			{
				// 最高点に到達した場合
				if (m_pos.y >= m_heightPosY)
				{
					m_pos.y = m_heightPosY; // 最高点に設定
					m_RiseFlag = false; // 上昇フラグを`false`に設定
				}
				else // 最高点に到達していない場合
				{
					m_pos.y += m_moveSpeed * 0.01f; // 上昇させる
				}
			}
		}
		else if (E_LiftState::Lift_DOWN)	// レバーが下に下がる処理
		{
			if (m_RiseFlag == true) // `true`になってたら
			{
				m_RiseFlag = false; // 上昇フラグを`false`に設定
			}
			// 上昇フラグが`false`の場合
			if (m_RiseFlag == false)
			{
				// 最低点に到達したら
				if (m_pos.y <= m_lowPosY)
				{
					m_pos.y = m_lowPosY; // 最低点に設定
				}
				else
				{
					m_pos.y -= m_moveSpeed * 0.01f; // 下降させる
				}
			}
		}
		else if (E_LiftState::Lift_STOP)	// リフトの途中停止
		{
			m_pos = m_oldPos;	// 今の位置に前の位置の情報を持たせておく
		}
	}
	else
	{
		// 上昇フラグが`true`の場合
		if (m_RiseFlag)
		{
			// 最高点に到達した場合
			if (m_pos.y >= m_heightPosY)
			{
				m_pos.y = m_heightPosY; // 最高点に設定
				m_RiseFlag = false; // 上昇フラグを`false`に設定
			}
			else // 最高点に到達していない場合
			{
				m_pos.y += m_moveSpeed * 0.01f; // 上昇させる
			}
		}
		else // 上昇フラグが`false`の場合
		{
			// 最低点に到達した場合
			if (m_pos.y <= m_lowPosY)
			{
				m_pos.y = m_lowPosY; // 最低点に設定
				m_RiseFlag = true; // 上昇フラグを`true`に設定
			}
			else // 最低点に到達していない場合
			{
				m_pos.y -= m_moveSpeed * 0.01f; // 下降させる
			}
		}
	}

	SetBounds(liftobj_MinBound, liftobj_MaxBound);
	CSetBounds(c_liftobj_MinBound, c_liftobj_MaxBound);
}

void Lift_Obj::Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	DirectX::XMFLOAT4X4 mat[3];

	// ワールド行列の設定
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(
				DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
				DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z)
				);
	DirectX::XMStoreFloat4x4(&mat[0], world);

	mat[1] = viewMatrix; // 引数の `viewMatrix` を利用
	mat[2] = projectionMatrix; // 引数の `projectionMatrix` を利用

	m_pVS->WriteBuffer(0, mat);
	m_pModel->Draw();
}

void Lift_Obj::SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
	minBound = Add(m_pos, min);
	maxBound = Add(m_pos, max);
}

DirectX::XMFLOAT3 Lift_Obj::GetminBounds()
{
	return liftobj_MinBound;
}

DirectX::XMFLOAT3 Lift_Obj::GetmaxBounds()
{
	return liftobj_MaxBound;
}

DirectX::XMFLOAT3 Lift_Obj::Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

void Lift_Obj::CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
{
	c_liftobj_MinBound = CAdd(m_pos, min);
	c_liftobj_MaxBound = CAdd(m_pos, max);
}

DirectX::XMFLOAT3 Lift_Obj::CGetminBounds()
{
	return c_liftobj_MinBound;
}

DirectX::XMFLOAT3 Lift_Obj::CGetmaxBounds()
{
	return c_liftobj_MaxBound;
}

DirectX::XMFLOAT3 Lift_Obj::CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
	DirectX::XMFLOAT3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
}

void Lift_Obj::Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
{
	m_pos.x = posX;
	m_pos.y = posY;
	m_pos.z = posZ;

	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
}

// ブロックとリフトが当たった時に返す
void Lift_Obj::LiftObj_SetPos()
{
	//m_pos = m_oldPos;
}

/**
 * @brief 最高点の位置を設定する
 * @param height  最高点の位置
 */
void Lift_Obj::SetHeightPosY(float height)
{
	m_heightPosY = height;
}

/**
 * @brief 最低点の位置を設定する
 * @param low 最低点の位置
 */
void Lift_Obj::SetLowPosY(float low)
{
	m_lowPosY = low;
}

/**
 * @brief 移動速度を設定する
 * @param speed 移動速度
 */
void Lift_Obj::SetSpeed(float speed)
{
	m_moveSpeed = speed;
}

void Lift_Obj::SetLever(Lever* pLever)
{
	g_pLever = pLever;
}

void Lift_Obj::SetMoveFlg(Lever::E_LeverState state)
{
	// レバーの状態とリフトの状態の連携
	m_LiftState = static_cast<E_LiftState>(state);	// 12/04 エラー箇所
}
