#pragma once

#include "Texture.h"

class Fade
{
public:
	Fade();
	~Fade();

	void Update();
	void Draw();

	void Start(bool isIn, float time);
	bool IsPlay();
	float GetAlpha();
private:
	Texture* m_pTex;

	float m_alpha;

	bool m_isIn;
	float m_time;
	float m_totalTime;
};

