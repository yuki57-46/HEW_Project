// インクルード部
#include "SceneManager.hpp"
#include "SceneGame.h"
#include "SceneTutorial.h"
#include "SceneResult.h"

SceneManager::SceneManager()
	: m_NowScene(SCENE_START)
	, m_IsEndGame(false)
	, m_pSceneGame(nullptr)
	, m_pSceneTutorial(nullptr)
	, m_pSceneResult(nullptr)
	, m_NextScene(SCENE_START)
{
	// メモリ確保
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTutorial = new SceneTutorial(this);

		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame = new SceneGame();

		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult = new SceneResult();

		break;
	}
}

SceneManager::~SceneManager()
{
	// メモリ開放
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		delete m_pSceneTutorial;
		m_pSceneTutorial = nullptr;

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
}

void SceneManager::Update(float tick)
{
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTutorial->Update(tick);

		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame->Update(tick);

		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult->Update(tick);

		break;
	}
}

void SceneManager::Draw()
{
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTutorial->Draw();

		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame->Draw();

		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult->Draw();

		break;
	}
}

// シーン切り替え
void SceneManager::ChangeScene(SCENE next)
{
	// メモリ開放
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		delete m_pSceneTutorial;
		m_pSceneTutorial = nullptr;

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

	m_NowScene = next;	// 次のシーンへ

	// メモリ確保
	switch (m_NowScene)
	{
	case SceneManager::SCENE_TITLE:
		m_pSceneTutorial = new SceneTutorial(this);

		break;

	case SceneManager::SCENE_GAME:
		m_pSceneGame = new SceneGame();

		break;

	case SceneManager::SCENE_RESULT:
		m_pSceneResult = new SceneResult();

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
