#include "LoadingThread.h"
#include "Scene/SceneManager.h"
#include "MainScene.h"
#include "Float1Scene.h"

CLoadingThread::CLoadingThread() :
	m_LoadingSceneType(LoadingSceneType::Green1)
{}

CLoadingThread::~CLoadingThread()
{}

bool CLoadingThread::Init()
{
	if (!CThread::Init())
		return false;

	return true;
}

void CLoadingThread::Run()
{
	switch (m_LoadingSceneType)
	{
	case LoadingSceneType::Green1 :
		{
			CSceneManager::GetInst()->CreateNewScene(false);

			CMainScene* MainScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CMainScene>(false);

			MainScene->SetLoadingFunction(this, &CLoadingThread::AddMessage);

			MainScene->Init();

			AddMessage(true, 1.f);
		}
		break;

	case LoadingSceneType::Float1:
	{
		CSceneManager::GetInst()->CreateNewScene(false);

		CFloat1Scene* Float1Scene = CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat1Scene>(false);

		Float1Scene->SetLoadingFunction(this, &CLoadingThread::AddMessage);

		// CSceneManager::GetInst()->GetNextScene()->PrepareResources();
		Float1Scene->PrepareResources();

		if (CSceneManager::GetInst()->GetNextScene()->Load("Float1.scn", SCENE_PATH))
		{
			AddMessage(true, 1.f);
			// CSceneManager::GetInst()->ChangeNextScene();
		}
	}
		break;
	}
}
