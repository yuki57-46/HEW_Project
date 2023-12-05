#pragma once
#include "Object.h"
#include "CameraBase.h"
#include "CameraDebug.h"
#include "Player.h"
#include "Collision.h"
#include"HCollision.h"
#include"col.h"
#include "Lift_Obj.hpp"

class ObjectMng
{
public:
	ObjectMng();
	~ObjectMng();
	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);


	
private:
	Object* m_pObjects;
	Lift_Obj* m_pLift;
	
	CameraDebug* m_pObjectCamera;
	Player* m_pPlayer;
	AABB* aabb;
	HAABB* haabb;
	CAABB* caabb;
	int m_num;
	
};
