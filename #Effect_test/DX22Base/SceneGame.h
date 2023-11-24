#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Model.h"
#include "Shader.h"
#include "CameraBase.h"
#include "Player.h"
#include "ItemUI.h"
#include "Effekseer_Effect.h"

class SceneGame
{
public:
	SceneGame();
	~SceneGame();
	void Update(float tick);
	void Draw();

private:
	Model* m_pModel;
	VertexShader* m_pVS;
	CameraBase* m_pCamera;
	Player* m_pPlayer;
	ItemUI* m_pUI;

	PixelShader* m_pPS;

	Effekseer_Effect* m_pEff;
};

#endif // __SCENE_GAME_H__