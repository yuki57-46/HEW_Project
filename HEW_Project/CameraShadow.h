#pragma once
//#ifndef ___CAMERA_SHADOW_H___
//#define ___CAMERA_SHADOW_H___
#include "CameraBase.h"
#include "BackShadow.h"

class CameraShadow
	: public CameraBase
{
public:
	CameraShadow();
	~CameraShadow();

	void Update();

private:
	//BackShadow* m_pBS;
	//カメラのパラメータ
	float m_radXZ;
	float m_radY;
	float m_radius;
};

//#endif // !___CAMERA_SHADOW_H___

