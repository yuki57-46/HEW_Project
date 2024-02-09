//// インクルード部
//#include "SceneManager.hpp"
//#include "TitleScene.h"
//#include "SceneGame.h"
//#include "SceneTutorial.h"
//#include "SceneResult.h"
//
//SceneManager::SceneManager()
//	: m_NowScene(SCENE_START)
//	, m_IsEndGame(false)
//	, m_pSceneTitle(nullptr)
//	, m_pSceneGame(nullptr)
//	, m_pSceneTutorial(nullptr)
//	, m_pSceneResult(nullptr)
//	, m_pFade(nullptr)
//	, m_pCurtainUI(nullptr)
//	, m_NextScene(SCENE_START)
//	, m_pSceneSelect(nullptr)
//	, m_IsFade(false)
//{
//	m_pCurtainUI	= new CurtainUI();
//	m_pFade			= new Fade(m_pCurtainUI);
//
//	m_pFade->Start(false, 0.5f);	// フェードイン
//// 
//	// メモリ確保
//	switch (m_NowScene)
//	{
//	case SceneManager::SCENE_TITLE:
//		m_pSceneTitle = new SceneTitle(this);
//		break;
//
//	case SceneManager::SCENE_TUTORIAL:
//		m_pSceneTutorial = new SceneTutorial(this);
//		break;
//
//	case SceneManager::SCENE_SELECT:
//		m_pSceneSelect = new SelectScene(this);
//		break;
//
//	case SceneManager::SCENE_GAME:
//		m_pSceneGame = new SceneGame(this);
//		break;
//
//	case SceneManager::SCENE_RESULT:
//		m_pSceneResult = new SceneResult(this);
//		break;
//	}
//}
//
//SceneManager::~SceneManager()
//{
//	// メモリ開放
//	switch (m_NowScene)
//	{
//	case SceneManager::SCENE_TITLE:
//		delete m_pSceneTitle;
//		m_pSceneTitle = nullptr;
//		break;
//
//	case SceneManager::SCENE_TUTORIAL:
//		delete m_pSceneTutorial;
//		m_pSceneTutorial = nullptr;
//		break;
//
//	case SceneManager::SCENE_SELECT:
//		delete m_pSceneSelect;
//		m_pSceneSelect = nullptr;
//		break;
//
//	case SceneManager::SCENE_GAME:
//		delete m_pSceneGame;
//		m_pSceneGame = nullptr;
//		break;
//
//	case SceneManager::SCENE_RESULT:
//		delete m_pSceneResult;
//		m_pSceneResult = nullptr;
//		break;
//
//	default:
//		break;
//	}
//	if (m_pFade)		{ delete m_pFade; }
//	if (m_pCurtainUI)	{ delete m_pCurtainUI;	m_pCurtainUI = nullptr; }
//}
//
//void SceneManager::Update(float tick)
//{
//	if (!m_pFade->IsPlay())
//	{
//		switch (m_NowScene)
//		{
//		case SceneManager::SCENE_TITLE:
//			m_pSceneTitle->Update();
//			break;
//
//		case SceneManager::SCENE_TUTORIAL:
//			m_pSceneTutorial->Update();
//			break;
//
//		case SceneManager::SCENE_SELECT:
//			m_pSceneSelect->Update();
//			break;
//
//		case SceneManager::SCENE_GAME:
//			m_pSceneGame->Update(tick);
//			break;
//
//		case SceneManager::SCENE_RESULT:
//			m_pSceneResult->Update();
//			break;
//		}
//	}
//	m_pFade->Update();
//	m_pCurtainUI->Update();	
//}
//
//void SceneManager::Draw()
//{
//	switch (m_NowScene)
//	{
//	case SceneManager::SCENE_TITLE:
//		m_pSceneTitle->Draw();
//		break;
//
//	case SceneManager::SCENE_TUTORIAL:
//		m_pSceneTutorial->Draw();
//		break;
//
//	case SceneManager::SCENE_SELECT:
//		m_pSceneSelect->Draw();
//		break;
//
//	case SceneManager::SCENE_GAME:
//		m_pSceneGame->Draw();
//		break;
//
//	case SceneManager::SCENE_RESULT:
//		m_pSceneResult->Draw();
//		break;
//	}
//
//	m_pFade->Draw();					// フェードの描画
//	m_pCurtainUI->StageCurtainDraw();	// カーテンの描画
//}
//
//// シーン切り替え
//void SceneManager::ChangeScene(SCENE next)
//{
//	// メモリ開放
//	switch (m_NowScene)
//	{
//	case SceneManager::SCENE_TITLE:
//		delete m_pSceneTitle;
//		m_pSceneTitle = nullptr;
//		break;
//
//	case SceneManager::SCENE_TUTORIAL:
//		delete m_pSceneTutorial;
//		m_pSceneTutorial = nullptr;
//		break;
//
//	case SceneManager::SCENE_SELECT:
//		delete m_pSceneSelect;
//		m_pSceneSelect = nullptr;
//		break;
//
//	case SceneManager::SCENE_GAME:
//		delete m_pSceneGame;
//		m_pSceneGame = nullptr;
//		break;
//
//	case SceneManager::SCENE_RESULT:
//		delete m_pSceneResult;
//		m_pSceneResult = nullptr;
//		break;
//	}
//
//	// 間隔は0.5秒で統一させる
//	m_pFade->Start(true, 0.5f);	// フェードイン
//	m_NowScene = next;
//
////	if (m_IsFade == true)
////	{
////		m_pFade->Start(true, 0.5f); // フェードイン
////		m_NowScene = next;
////	}
//	
//	// メモリ確保
//	switch (m_NowScene)
//	{
//	case SceneManager::SCENE_TITLE:
//		m_pSceneTitle = new SceneTitle(this);
//		break;
//
//	case SceneManager::SCENE_TUTORIAL:
//		m_pSceneTutorial = new SceneTutorial(this);
//		break;
//
//	case SceneManager::SCENE_SELECT:
//		m_pSceneSelect = new SelectScene(this);
//		break;
//
//	case SceneManager::SCENE_GAME:
//		m_pSceneGame = new SceneGame(this);
//		break;
//
//	case SceneManager::SCENE_RESULT:
//		m_pSceneResult = new SceneResult(this);
//		break;
//	}
//}
//
//bool SceneManager::IsFade()
//{
//	// フェードが続いているかどうか
//	if (m_IsFade && m_pFade->IsPlay())
//	{
//		m_pFade->Start(false, 0.5f);  // フェードアウト
//		return true;
//	}
//	return false;
//}
//
//void SceneManager::NextScene()
//{
//	//if (!m_pFade->IsPlay())
//	//{
//	//	// フェードアウト
//	//	m_pFade->Start(false, 0.5f);
//	//	m_NowScene = next;
//	//}
//}
//
//// ゲームの終了
//void SceneManager::EndGame()
//{
//	m_IsEndGame = true;
//}
//
//// ゲームの終了処理
//bool SceneManager::IsEndGame()
//{
//	return m_IsEndGame;
//}
#include "SceneManager.hpp"
#include "TitleScene.h"
#include "SceneTutorial.h"
#include "SceneGame.h"
#include "SceneResult.h"

SceneManager::SceneManager()
	: m_Nowscene(SCENE_MAX)
	, m_NextScene(SCENE_START)
	, m_pSceneTitle(nullptr)
	, m_pSceneTutorial(nullptr)
	, m_pSceneGame(nullptr)
	, m_pSceneResult(nullptr)
	, m_pFade(nullptr)
	, m_pCurtainUI(nullptr)
{
	m_pCurtainUI = new CurtainUI();
	m_pFade = new Fade(m_pCurtainUI);
}

SceneManager::~SceneManager()
{
	switch (m_Nowscene)
	{
	case SCENE_TITLE:	 delete m_pSceneGame;		break;
	case SCENE_TUTORIAL: delete m_pSceneTutorial;	break;
	case SCENE_GAME:	 delete m_pSceneGame;		break;
	case SCENE_RESULT:	 delete m_pSceneResult;		break;
	default: break;
	}
	delete m_pFade;
	delete m_pCurtainUI;
}

void SceneManager::Update(float tick)
{
	if (!m_pFade->IsPlay())
	{
		// シーンの切り替え判定
		if (m_Nowscene != m_NextScene)
		{
			// 現在のシーンを削除
			switch (m_Nowscene)
			{
			case SCENE_TITLE:	 delete m_pSceneTitle;		break;
			case SCENE_TUTORIAL: delete m_pSceneTutorial;	break;
			case SCENE_GAME:	 delete m_pSceneGame;		break;
			case SCENE_RESULT:	 delete m_pSceneResult;		break;
			default: break;
			}
			// 新しいシーンの読込
			switch (m_NextScene)
			{
			case SCENE_TITLE:	 m_pSceneTitle = new SceneTitle();			break;
			case SCENE_TUTORIAL: m_pSceneTutorial = new SceneTutorial();	break;
			case SCENE_GAME:	 m_pSceneGame = new SceneGame();			break;
			case SCENE_RESULT:	 m_pSceneResult = new SceneResult();		break;
			default: break;
			}
			// 現在のシーンを新しいシーンへ上書き
			m_Nowscene = m_NextScene;					
			m_pFade->Start(true, 1.0f);	// フェードイン
			
		}
		else
		{
			// フェードの明るさにあわせて、スピーカーのボリュームを調節
			//m_pSpeaker->SetVolume(1.0f - m_pFade->GetAlpha());
		}
	}
	switch (m_Nowscene)
	{
	case SCENE_TITLE:	 m_pSceneTitle->Update(this);		break;
	case SCENE_TUTORIAL: m_pSceneTutorial->Update(this);	break;
	case SCENE_GAME:	 m_pSceneGame->Update(this, tick);	break;
	case SCENE_RESULT:   m_pSceneResult->Update(this);		break;
	default: break;
	}
	// フェードの更新処理

	m_pFade->Update();
	m_pCurtainUI->Update();
}

void SceneManager::Draw()
{
	switch (m_Nowscene)
	{
	case SCENE_TITLE:	 m_pSceneTitle->Draw();		break;
	case SCENE_TUTORIAL: m_pSceneTutorial->Draw();	break;
	case SCENE_GAME:	 m_pSceneGame->Draw();		break;
	case SCENE_RESULT:   m_pSceneResult->Draw();	break;
	default: break;
	}
	// 一番最後に画面全体に表示する
	m_pFade->Draw();
	m_pCurtainUI->StageCurtainDraw();
}

void SceneManager::SetNextScene(SceneKind scene)
{
	//m_pFade->Start(false, 1.0f);	// フェードアウト
	// フェード中は次のシーンを予約しない
	m_NextScene = scene;	

}
