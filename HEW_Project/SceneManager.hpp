﻿//#ifndef __SCENE_MANAGER_H__
//#define __SCENE_MANAGER_H__
//
//// インクルード部
//#include "SceneGame.h"
//#include "SceneTutorial.h"
//#include "SceneResult.h"
//#include "TitleScene.h"
//#include "SelectScene.h"
//
//// フェード関連インクルード部
//#include "Fade.hpp"
//#include "Curtain.h"
//
//// クラス定義
//class SceneGame;
//class SceneTutorial;
//class SceneResult;
//class SceneTitle;
//class SelectScene;
//
//class SceneManager
//{
//public:
//	enum SCENE
//	{
//		SCENE_TITLE = 0,	// タイトル
//		SCENE_TUTORIAL,		// チュートリアル(操作説明)
//		SCENE_SELECT,		// ステージ選択
//
//		SCENE_GAME,			// ゲーム、第１ステージ
//
//		//SCENE_STAGE2,		// 第２ステージ
//		//SCENE_STAGE3,		// 第３ステージ
//		//SCENE_STAGE4,		// 第４ステージ
//
//		SCENE_RESULT,		// リザルト
////		SCENE_CLEAR,		// リザルト（ゲームクリア）
////		SCENE_GAMEOVER,		// リザルト（ゲームオーバー）
//
//		SCENE_MAX,			// シーンの数
//		SCENE_START = SCENE_TITLE	// 最初に出すシーンの画面
//	};
//
//public:
//	SceneManager();
//	~SceneManager();
//
//	void Update(float tick);
//	void Draw();
//
//	void ChangeScene(SCENE next);	// シーンの切り替え
//	void NextScene();				// フェード用
//	void EndGame();					// ゲームの終了
//	bool IsEndGame();				// ゲームが続いているか
//	bool IsFade();					// フェードが続いているか
//
//private:
//	bool m_IsEndGame;	// 終了判定
//	bool m_IsFade;		// シーン切り替え用
//
//	// シーン
//	SceneTitle*		m_pSceneTitle;
//	SceneTutorial*	m_pSceneTutorial;
//	SelectScene*	m_pSceneSelect;
//	SceneGame*		m_pSceneGame;
//	SceneResult*	m_pSceneResult;
//
//	// フェード関連
//	Fade*		m_pFade;
//	CurtainUI*	m_pCurtainUI;
//
//	SCENE m_NowScene;	// 現在のシーン
//	SCENE m_NextScene;	// 次のシーン
//};
//#endif // !__SCENE_MANAGER_H__

#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Fade.hpp"
#include "Curtain.h"

// 相互インクルードが発生した場合やむを得ない対応
class SceneTitle;
class SceneTutorial;
class SelectScene;
class SceneGame;
class SceneResult;
class SelectScene;

enum SceneKind
{
	SCENE_TITLE = 0,
	SCENE_TUTORIAL,
	SCENE_SELECT,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX,

	SCENE_START = SCENE_TITLE,
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Update(float tick);
	void Draw();

	void SetNextScene(SceneKind scene);
private:
	int		m_Nowscene; // 現在実行中のシーン
	int		m_NextScene; // 次のシーン
	int		m_SelectScene; // 選択されたシーン

	Fade*		m_pFade;
	CurtainUI*	m_pCurtainUI;

	SceneTitle*		m_pSceneTitle;
	SceneTutorial*	m_pSceneTutorial;
	SelectScene*	m_pSceneSelect;
	SceneGame*		m_pSceneGame;
	SceneResult*	m_pSceneResult;
};

#endif // !__SCENE_MANAGER_H__
