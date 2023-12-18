#pragma once
// Player.h
#include "Model.h"

#include "Collision.h"
#include "HCollision.h"
#include "GameObject.h"

//#include"Object.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBox();
	DirectX::XMFLOAT3 GetmaxBox();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 HGetminBox();
	DirectX::XMFLOAT3 HGetmaxBox();
	DirectX::XMFLOAT3 HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);


	void PlayerPos();
	void HPlayerPos();
	void RPlayerPos();


	bool Set();
	void SetOk();
	void SetNOk();

private:
	Model* m_pModel;
	VertexShader* m_pVS;

  
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_oldPos;
	float m_rotationY;
	float  m_lastFacingDirection;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	bool IsUse;
	bool ok;
	
};
