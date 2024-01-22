#pragma once
#ifndef __SELECT_H__
#define __SELECT_H__

#include "Sprite.h"
#include "Texture.h"

class Select
{
public:
	Select();
	~Select();
	void UpdateTitle();
	void DrawTitle();

private:
	Texture* m_pTexture;
};

#endif
