//Curtain.h
//カーテン表示UI

#pragma once
#ifndef __CURTAIN_UI_H__
#define __CURTAIN_UI_H__

#include "Texture.h"		//Texture.hの導入

class CurtainUI				//カーテンクラス
{
public:
	CurtainUI();			//コンストラクタ
	~CurtainUI();			//デストラクタ

	void Update();			//更新
	void LeftDraw();			//描画
	void RightDraw();

private:
	Texture* m_pTexture;
};
#endif
