#pragma once
// ShadowP.h
#include "Model.h"
#include "CameraDebug.h"
#include "CameraBase.h"
#include "Collision.h"
#include "GameObject.h"

class ShadowP : public GameObject
{
public:
	ShadowP();
	~ShadowP();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);
	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);
	void SetPos(float x, float y, float z);
	void ShadowPPosY();
	void oldX();
	float ShadowPPosX();
	DirectX::XMFLOAT3 NowPos();
	void ShadowPPos();
	void Use();
	void NotUse();
	void Jump();
	bool isUse();
	bool IsJump();
	//bool IsJump();
private:
	Model* m_pModel;
	VertexShader* m_pVS;
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_oldPos;
	bool IsUse;
	bool m_Jump;
	float m_moveY;
	float m_JumpY;
};

