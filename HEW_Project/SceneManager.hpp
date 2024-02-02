#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

// インクルード部
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneResult.h"
#include "TitleScene.h"
#include "SelectScene.h"

// クラス定義
class SceneGame;
class SceneTutorial;
class SceneResult;
class SceneTitle;
class SelectScene;

class SceneManager
{
public:
	enum SCENE
	{
		SCENE_TITLE = 0,	// タイトル
		SCENE_TUTORIAL,		// チュートリアル(操作説明)

		SCENE_SELECT,		// ステージ選択

		SCENE_GAME,			// ゲーム、第１ステージ
		//SCENE_STAGE2,		// 第２ステージ
		//SCENE_STAGE3,		// 第３ステージ
		//SCENE_STAGE4,		// 第４ステージ

		SCENE_RESULT,		// リザルト
//		SCENE_CLEAR,		// リザルト（ゲームクリア）
//		SCENE_GAMEOVER,		// リザルト（ゲームオーバー）

		SCENE_MAX,			// シーンの数

		SCENE_START = SCENE_SELECT	// 最初に出すシーンの画面
	};

public:
	SceneManager();
	~SceneManager();

	void Update(float tick);
	void Draw();

	void ChangeScene(SCENE next);	// シーンの切り替え
	void EndGame();					// ゲームの終了
	bool IsEndGame();				// ゲームが続いているか

private:
	bool m_IsEndGame;	// 終了判定

	// シーン
	SceneTitle*		m_pSceneTitle;
	SceneTutorial*	m_pSceneTutorial;
	SelectScene*	m_pSceneSelect;
	SceneGame*		m_pSceneGame;



	SceneResult*	m_pSceneResult;


	SCENE m_NowScene;	// 現在のシーン
	SCENE m_NextScene;	// 次のシーン

};

#endif // !__SCENE_MANAGER_H__
