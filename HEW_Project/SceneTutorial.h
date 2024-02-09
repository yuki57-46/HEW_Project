﻿#ifndef __SCENE_TUTORIAL_H__
#define __SCENE_TUTORIAL_H__

// インクルード部
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"

class SceneManager;
//#include "SceneManager.hpp"

#include "Model.h"
#include "Shader.h"
#include "CameraBase.h"
#include "Objectcamera.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Object.h"
#include"CameraObject.h"

#include"BackShadow.h"
#include "Soundtest.h"
#include"Coin.h"
#include"CoinCntUI.h"
#include"ItemUI.h"

enum class CameraKindTutorial
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SceneTutorial
{
public:
	SceneTutorial();
	~SceneTutorial();
	void Update(SceneManager* pSceneManager);
	void Draw();

private:
	Texture*		m_pTexture1;
	Texture*		m_pTexture2;
	Texture*		m_pTexture3;

	Screen* m_pScreen;

	ObjectMng* m_pObjectMng;
	VertexShader* m_pVS;

	ObjectCamera* m_pobjcamera;

	CameraKindTutorial	m_mainCamera;
	CameraBase*		m_pCamera[static_cast<int>(CameraKindTutorial::MAX_CAMERA)];

	BackShadow* m_pBackShadow;

	ItemUI* m_pUI;
	Coin* m_pCoin;
	CoinCntUI* m_pCoinCntUI;


	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	PixelShader* m_pPS;

	IXAudio2SourceVoice* m_pSourceVoice; // サウンドソース
	XAUDIO2_BUFFER* m_pSound; // サウンドバッファ
	//Sound m_Sound;
};

#endif // __SCENE_TUTORIAL_H__

