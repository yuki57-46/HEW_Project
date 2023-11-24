#pragma once

#include "LibEffekseer.h"
#include "CameraBase.h"

class Effekseer_Effect
{
public:
	Effekseer_Effect();
	~Effekseer_Effect();

	void Update();
	void Draw();

	void SetCamera(CameraBase* pCamera) { m_pCamera = pCamera; }


private:
	Effekseer::EffectRef effect;
	CameraBase* m_pCamera;
};

