#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Model.h"
#include "Shader.h"
#include "CameraBase.h"
#include "CameraDebug.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Object.h"

#include "ShadowP.h"
#include "2DObjectManager.h"
class SceneGame
{
public:
	SceneGame();
	~SceneGame();
	void Update(float tick);
	void Draw();

private:
	Player* m_pPlayer;
	ObjectMng* m_pObjectMng;
	VertexShader* m_pVS;
	CameraDebug* m_pCamera;

	ShadowP* m_pShadowP;
	Object2D* m_pObject2D;
};

#endif // __SCENE_GAME_H__