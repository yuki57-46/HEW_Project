//CoinCntUI.h
//�W�߂��R�C����\������UI
//Cnt��Count�̗�

#pragma once
#ifndef __COIN_CNT_UI_H__
#define __COIN_CNT_UI_H__

#include "Texture.h"		//Texture.h�̓���

class CoinCntUI				//�W�߂��R�C���N���X
{
public:
	CoinCntUI();			//�R���X�g���N�^
	~CoinCntUI();			//�f�X�g���N�^

	void Update();			//�X�V
	void Draw();			//�`��
	
private:
	Texture* m_pTexture;
};
#endif