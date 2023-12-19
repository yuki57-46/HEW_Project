#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

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


enum CameraKind
{
	CAM_OBJ,	//
	CAM_SHADOW,	//�����_�[�p�J����
	CAM_DEBUG,	//�Ńo�b�N�p�̃J����
	MAX_CAMERA	//�J�����ő吔
};


class SceneGame
{
public:
	SceneGame();
	~SceneGame();
	void Update(float tick);
	void Draw();

private:

	ObjectMng* m_pObjectMng;
	VertexShader* m_pVS;

	ObjectCamera* m_pobjcamera;

	CameraKind		m_mainCamera;
	CameraBase*		m_pCamera[MAX_CAMERA];

	BackShadow*		m_pBackShadow;

	
	


	RenderTarget*	m_pRTV;
	DepthStencil*	m_pDSV;

	IXAudio2SourceVoice* m_pSourceVoice; // �T�E���h�\�[�X
	XAUDIO2_BUFFER* m_pSound; // �T�E���h�o�b�t�@
	//Sound m_Sound;
};

#endif // __SCENE_GAME_H__