#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__
#include "Model.h"
#include "Shader.h"
#include "ShaderList.h"
#include "CameraBase.h"
#include "Player.h"
#include "ItemUI.h"

class SceneGame
{
public:
	SceneGame();
	~SceneGame();
	void Update(float tick);
	void Draw();

private:
	VertexShader*	m_pVS;
	Model*			m_pModel;
	CameraBase*		m_pCamera;
	Player*			m_pPlayer;
	ItemUI*			m_pUI;
};

#endif // __SCENE_GAME_H__