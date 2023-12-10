#pragma once
#include "CameraBase.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"

class ObjectAutoMove : public GameObject
{
public:
	ObjectAutoMove();
	~ObjectAutoMove();
	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	void SetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 GetminBounds();
	DirectX::XMFLOAT3 GetmaxBounds();
	DirectX::XMFLOAT3 Add(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void HSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 HGetminBounds();
	DirectX::XMFLOAT3 HGetmaxBounds();
	DirectX::XMFLOAT3 HAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void CSetBounds(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max);
	DirectX::XMFLOAT3 CGetminBounds();
	DirectX::XMFLOAT3 CGetmaxBounds();
	DirectX::XMFLOAT3 CAdd(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

	void CreateAuto(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ);

	void Set();
	void SetF();
	bool SetR();

	void OBJPos();
//	bool OBJRPos();

	void Modelchg();
	void Modelchg2();

private:
	Model*				m_pObjectModel;
	VertexShader*		m_pObjectVS;
	DirectX::XMFLOAT3	m_oldPos;
	DirectX::XMFLOAT3	m_pos;
	DirectX::XMFLOAT3	m_scale;

	float m_rotationY;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMMATRIX m_rotationMatrix;
	float m_lastRotationY;
	bool moveok;

	float m_MaxPosX;	// X���̌��E�͈�(�E)
	float m_MinPosX;	// X���̌��E�͈�(��)

	bool m_MoveX;	// �I�u�W�F�N�g�̎�������

	bool m_isPossessed;
};