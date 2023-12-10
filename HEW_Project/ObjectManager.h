#pragma once
#include "Object.h"
#include "ObjectNot.h"
#include "ObjectAutoMove.h"
#include "CameraBase.h"
#include "CameraDebug.h"
#include "Player.h"
#include "Collision.h"
#include "HCollision.h"
#include "col.h"

class ObjectMng
{
public:
	ObjectMng();
	~ObjectMng();
	void Update();
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix);


private:
	Object*			m_pObjects;
	ObjectNot*		m_pObjectsNot;
	ObjectAutoMove* m_pObjectsAuto;

	CameraDebug* m_pObjectCamera;
	Player* m_pPlayer;
	AABB* aabb;
	HAABB* haabb;
	CAABB* caabb;
	int m_num;

};
