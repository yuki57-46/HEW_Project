#ifndef __SCENE_RESULT_H__
#define __SCENE_RESULT_H__

// インクルード部
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
	void Update(SceneManager* pSceneManager);
	void BGDraw();
	void ClearDraw();
	void FPDraw();
	void NextDraw();

private:
	Texture*		 m_pBGTexture;
	Texture*		 m_pClearIcon;
	Texture*		 m_pFPIcon;
	Texture*		 m_pNextIcon;

	SceneManager*	 m_pSceneManager;
	Fade*			 m_pFade;
	Screen*			 m_pScreen;

	VertexShader*	 m_pVS;
	PixelShader*	 m_pPS;

	ObjectCamera*	 m_pobjcamera;
	CameraKindResult m_mainCamera;
	CameraBase*		 m_pCamera[static_cast<int>(CameraKindResult::MAX_CAMERA)];

	CurtainUI*		 m_pCurtainUI;

	RenderTarget* m_pRTV;
	DepthStencil* m_pDSV;

	IXAudio2SourceVoice* m_pSourceVoice; // サウンドソース
	XAUDIO2_BUFFER*		 m_pSound; // サウンドバッファ
	//Sound m_Sound;
};

#endif // __SCENE_RESULT_H__

