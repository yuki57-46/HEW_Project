﻿#pragma once
#ifndef __COIN_H__
#define __COIN_H__

#include "Texture.h"
#include "Collision.h"
#include "Shader.h"

class Coin
{
public:
	Coin();
	~Coin();

	void Update();
	void Draw(float x, float y, float z, float sizeX, float sizeY, int num);

	bool IsCoinCollected = false;							// コインが取得されたかどうかを返す

	void SetPosition(float x, float y, float z);			// コイン位置を設定
	DirectX::XMFLOAT3 GetPosition() const;

	float GetSizeX(float x);								// コインの横幅を取得
	float GetSizeY(float y);								// コインの縦幅を取得
	DirectX::XMFLOAT3 GetSize() const;						// コインのサイズを取得

	void SetCollect(bool CollectFirst);
	bool GetCollect();

private:
	Texture* m_pCFirstTexture;
	Texture* m_pCSecondTexture;
	Texture* m_pCThirdTexture;

	DirectX::XMFLOAT3 m_CoinPos;							// コインの現在の位置
	DirectX::XMFLOAT3 m_CoinSize;							//コインの大きさ

	PixelShader* m_pPS;
};


#endif
