﻿#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__

#include "Model.h"
#include "Shader.h"

#include "CameraBase.h"
#include"Objectcamera.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Object.h"
//#include"CameraObject.h"

#include"BackShadow.h"

#include "Soundtest.h"

#include"Coin.h"
#include"CoinCntUI.h"
#include"ItemUI.h"

enum class CameraKindResult
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SceneResult
{
public:
	SceneResult();
	~SceneResult();
	void Update(float tick);
	void Draw();

private:

	Screen* m_pScreen;

	ObjectMng* m_pObjectMng;
	VertexShader* m_pVS;

	ObjectCamera* m_pobjcamera;

	CameraKindResult m_mainCamera;
	CameraBase*		m_pCamera[static_cast<int>(CameraKindResult::MAX_CAMERA)];

	BackShadow* m_pBackShadow;

	ItemUI* m_pUI;
	Coin* m_pCoin;
	CoinCntUI* m_pCoinCntUI;


	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	IXAudio2SourceVoice* m_pSourceVoice; // サウンドソース
	XAUDIO2_BUFFER* m_pSound; // サウンドバッファ
	//Sound m_Sound;
};

#endif // __SCENE_RESULT_H__

