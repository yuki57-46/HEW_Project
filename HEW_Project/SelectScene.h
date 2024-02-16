﻿#pragma once
#ifndef __SELECT_SCENE_H__
#define __SELECT_SCENE_H__

//インクルード部
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"
#include "Fade.hpp"
#include "Shader.h"
#include "CameraBase.h"
#include "Objectcamera.h"
#include "CameraObject.h"

#include "BackShadow.h"
#include "Soundtest.h"

class SceneManager;

enum class CameraKindSelect
{
	CAM_OBJ,	//
	CAM_SHADOW,	//レンダー用カメラ
	CAM_DEBUG,	//でバック用のカメラ
	MAX_CAMERA	//カメラ最大数
};


class SelectScene
{
public:
	SelectScene();
	~SelectScene();
	void Update(SceneManager* pSceneManager);
	void Draw();

private:
	Texture*		 m_pTexture;
	SceneManager*	 m_pSceneManager;
	Fade*			 m_pFade;
	Screen*			 m_pScreen;

	VertexShader*	 m_pVS;
	PixelShader*	 m_pPS;

	ObjectCamera*	 m_pobjcamera;
	CameraKindSelect m_mainCamera;
	CameraBase*		m_pCamera[static_cast<int>(CameraKindSelect::MAX_CAMERA)];

	CurtainUI*		 m_pCurtainUI;

	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	IXAudio2SourceVoice* m_pSourceVoice;	 // サウンドソース
	XAUDIO2_BUFFER*		 m_pSound;			 // サウンドバッファ
	//Sound m_Sound;
};


#endif
