#pragma once
#ifndef __ITEM__UI_H__
#define __ITEM__UI_H__

#include "Texture.h"

class ItemUI
{
public:
	ItemUI();
	~ItemUI();

	void Update();
	void Draw();

private:
	Texture*	m_pTexture;
};


#endif // !__ITEMUI_H__

