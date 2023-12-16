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
	struct Setting
	{
		float x, y, z, scaleX, scaleY, scaleZ;
		int kind;	// �I�u�W�F�N�g�̎��
	};

	enum BlockType
	{
		BLOCK_H,	// �߈ˉ\�u���b�N
		BLOCK_D,	// �߈˕s�u���b�N
		BLOCK_M,	// ���E�ړ��u���b�N
	};

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
