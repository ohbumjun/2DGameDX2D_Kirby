#include "LoadingThread.h"
#include "Scene/SceneManager.h"
#include "MainScene.h"

CLoadingThread::CLoadingThread()
{}

CLoadingThread::~CLoadingThread()
{}

bool CLoadingThread::Init()
{
	// 쓰레드 생성
	if (!CThread::Init())
		return false;

	return true;
}

void CLoadingThread::Run()
{
	CSceneManager::GetInst()->CreateNewScene(false);
	CMainScene* MainScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CMainScene>(false);

	MainScene->SetLoadingFunction(this, &CLoadingThread::AddMessage);

	MainScene->Init();

	AddMessage(false, 1.f);

	Sleep(1000);

	AddMessage(true, 1.f);
}
