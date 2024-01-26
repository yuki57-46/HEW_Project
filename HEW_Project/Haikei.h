#pragma once
//Curtain.h
//カーテン表示UI

#pragma once
#ifndef __Haikei_UI_H__
#define __Haikei_UI_H__

#include "Texture.h"		//Texture.hの導入

class Haikei				//カーテンクラス
{
public:
	Haikei();			//コンストラクタ
	~Haikei();			//デストラクタ

	void Update();			//更新
	void Draw();			//描画


private:
	Texture* m_pTexture;	//テクスチャ

	

};
#endif
