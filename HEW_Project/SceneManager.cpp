// インクルード部
#include "SceneManager.hpp"
#include "TitleScene.h"
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneResult.h"

SceneManager::SceneManager()
	: m_NowScene(SCENE_START)
	, m_IsEndGame(false)
	, m_pSceneTitle(nullptr)
	, m_pSceneGame(nullptr)
	, m_pSceneTutorial(nullptr)
	, m_pSceneResult(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
	, m_NextScene(SCENE_START)
	, m_pSceneSelect(nullptr)
	, m_bIsButton(false)
{
	m_pCurtainUI = new CurtainUI();
	m_pFade	= new Fade(m_pCurtainUI);

	// メモリ確保
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTitle = new SceneTitle(this);
		break;

	case SceneManager::SCENE_TUTORIAL:
		m_pSceneTutorial = new SceneTutorial(this);
		break;

	case SceneManager::SCENE_SELECT:
		m_pSceneSelect = new SelectScene(this);
		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame = new SceneGame(this);
		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult = new SceneResult(this);
		break;
	}
}

SceneManager::~SceneManager()
{
	// メモリ開放
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		delete m_pSceneTitle;
		m_pSceneTitle = nullptr;
		break;

	case SceneManager::SCENE_TUTORIAL:
		delete m_pSceneTutorial;
		m_pSceneTutorial = nullptr;
		break;

	case SceneManager::SCENE_SELECT:
		delete m_pSceneSelect;
		m_pSceneSelect = nullptr;
		break;

	case SceneManager::SCENE_GAME:
		delete m_pSceneGame;
		m_pSceneGame = nullptr;
		break;

	case SceneManager::SCENE_RESULT:
		delete m_pSceneResult;
		m_pSceneResult = nullptr;
		break;

	default:
		break;
	}
	if (m_pFade)		{ delete m_pFade; }
	if (m_pCurtainUI)	{ delete m_pCurtainUI;	m_pCurtainUI = nullptr; }
}

void SceneManager::Update(float tick)
{
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTitle->Update();
		break;

	case SceneManager::SCENE_TUTORIAL:
		m_pSceneTutorial->Update();
		break;

	case SceneManager::SCENE_SELECT:
		m_pSceneSelect->Update();
		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame->Update(tick);
		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult->Update();
		break;
	}
	m_pFade->Update();
	m_pCurtainUI->Update();

	//if (m_bIsButton == true)
	//{
	//	if (!m_pFade->IsPlay())
	//	{
	//		m_NowScene = next;	// 次のシーンへ
	//		m_bIsButton = false;
	//	}
	//}
}

void SceneManager::Draw()
{
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTitle->Draw();
		break;

	case SceneManager::SCENE_TUTORIAL:
		m_pSceneTutorial->Draw();
		break;

	case SceneManager::SCENE_SELECT:
		m_pSceneSelect->Draw();
		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame->Draw();
		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult->Draw();
		break;
	}
	m_pFade->Draw();	// フェードの描画
	m_pCurtainUI->StageCurtainDraw();	// カーテンの描画
}

// シーン切り替え
void SceneManager::ChangeScene(SCENE next)
{
	// メモリ開放
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		delete m_pSceneTitle;
		m_pSceneTitle = nullptr;
		break;

	case SceneManager::SCENE_TUTORIAL:
		delete m_pSceneTutorial;
		m_pSceneTutorial = nullptr;
		break;

	case SceneManager::SCENE_SELECT:
		delete m_pSceneSelect;
		m_pSceneSelect = nullptr;
		break;

	case SceneManager::SCENE_GAME:
		delete m_pSceneGame;
		m_pSceneGame = nullptr;
		break;

	case SceneManager::SCENE_RESULT:
		delete m_pSceneResult;
		m_pSceneResult = nullptr;
		break;
	}
//	m_pFade->Start(false, 1.0f); // フェードアウト
	//if (IsB == true)
	//{
	//	if (!m_pFade->IsPlay())
	//	{
	//		m_NowScene = next;	// 次のシーンへ
	//		IsB = false;
	//	}
	//}
	m_NowScene = next;
	// 間隔は0.5秒で統一させる
	m_pFade->Start(true, 0.5f);	// フェードイン

	// メモリ確保
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTitle = new SceneTitle(this);
		break;

	case SceneManager::SCENE_TUTORIAL:
		m_pSceneTutorial = new SceneTutorial(this);
		break;

	case SceneManager::SCENE_SELECT:
		m_pSceneSelect = new SelectScene(this);
		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame = new SceneGame(this);
		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult = new SceneResult(this);
		break;
	}
}

// ゲームの終了
void SceneManager::EndGame()
{
	m_IsEndGame = true;
}

// ゲームの終了処理
bool SceneManager::IsEndGame()
{
	return m_IsEndGame;
}
