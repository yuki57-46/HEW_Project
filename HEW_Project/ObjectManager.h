#pragma once
#include "Object.h"
// 1/23 追加オブジェクト

//#include "ObjectAutoMove.h"

#include "yuka.h"
#include "CameraBase.h"
#include "CameraDebug.h"
#include"CameraObject.h"
#include "Player.h"
#include "Collision.h"
#include "HCollision.h"
#include "col.h"
#include "Lift_Obj.hpp"
#include "Lever.h"
#include "Stair.h"
#include "LibEffekseer.h"
#include "Effekseer/Effekseer.h"



class ObjectMng
{
public:
	ObjectMng();
	~ObjectMng();
	void Update(float tick);
	void Draw(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix,bool draw);
	
private:
	Object*			m_pObjects;
	
	//ObjectAuto*		m_pObjectsAuto;

	Yuka*		m_pYuka;
	Lift_Obj*	m_pLift_obj;
	Lever*		m_pLever;
	Stair*		m_pStair;

	CameraDebug* m_pObjectCamera;

	Player* m_pPlayer;
	AABB* aabb;
	HAABB* haabb;
	CAABB* caabb;
	Effekseer::Handle		m_EffectHandle;
	Effekseer::EffectRef	m_Effect;

	int m_num;	// ブロック
	int m_num1;	// リフト
	int m_num2;	// レバー
	int m_num3;	// ゴール
	int m_num4;	// 床

	int m_num6;	// 動くブロック
};
