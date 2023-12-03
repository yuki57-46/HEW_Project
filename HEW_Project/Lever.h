#pragma once
//#include <DirectXMath.h>
//#include "2DObjectManager.h"
//#include "Player.h"

#include "GameObject.h"
#include "Model.h"
#include "Shader.h"

class Lever : public GameObject
{
public:
	Lever();
	~Lever();
	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);

	bool GetLeverFlag();

	void Create(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, bool isFlag = false);

	void ModelChange();
	void ModelChange2();

private:
	bool m_isUse;

	Model* m_pModel;	// モデル
	VertexShader* m_pVS; // 頂点シェーダー
	DirectX::XMFLOAT3 m_pos;	// 位置
	DirectX::XMFLOAT3 m_scale;	// 大きさ
	DirectX::XMFLOAT3 m_rot;	// 回転量
	DirectX::XMMATRIX m_rotMat;	// 回転行列
	DirectX::XMFLOAT3 m_direction;	// 向き
	float m_rotSpeed = 10.0f;	// 回転速度の設定


	class Lift_Obj* m_pLift_Obj;
};
