#pragma once

#include "GameObject.h"
#include "Object.h"
#include "Model.h"
#include "Collision.h"

class Lift_Obj : public GameObject, public Object
{
public:
	Lift_Obj();
	~Lift_Obj();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBounds();
	DirectX::XMFLOAT3 GetmaxBounds();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 CGetminBounds();
	DirectX::XMFLOAT3 CGetmaxBounds();
	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void LiftObj_SetPos();

private:
	Model* m_pModel;	// モデル
	VertexShader* m_pVS; // 頂点シェーダー

	DirectX::XMFLOAT3 m_pos;	// 位置
	DirectX::XMFLOAT3 m_oldPos;	// 前にいた位置
	DirectX::XMFLOAT3 m_scale;	// 大きさ

	float m_rotY;	// 回転量
	DirectX::XMFLOAT3 m_direction;	// 向き
	DirectX::XMMATRIX m_rotMat;	// 回転行列
	float m_lastRotY;	// 前の回転量
	bool moveOk;	// 移動可能かどうか

};

