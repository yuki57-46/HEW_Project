﻿//CoinCntUI.h
//集めたコインを表示するUI
//CntはCountの略

#pragma once
#ifndef __COIN_CNT_UI_H__
#define __COIN_CNT_UI_H__

#include "Texture.h"		//Texture.hの導入


class CoinCntUI				//集めたコインクラス
{
public:
	CoinCntUI();			//コンストラクタ
	~CoinCntUI();			//デストラクタ

	void Update();			//更新
	void Draw();			//描画

	void GoalDraw();//ゴールの仮処理用

private:
	Texture* m_pTexture;
	Texture* m_pGT;	//ゴールの仮処理用
};
#endif
