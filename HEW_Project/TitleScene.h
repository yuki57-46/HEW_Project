//TitleScene.h
#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Sprite.h"
#include "Texture.h"

class TitleScene
{
public:
	TitleScene();
	~TitleScene();
	void UpdateTitle();
	void DrawTitle();

private:
	Texture* m_pTexture;
};

#endif