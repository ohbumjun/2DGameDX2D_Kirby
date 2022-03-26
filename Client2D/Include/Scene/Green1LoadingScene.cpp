#include "Green1LoadingScene.h"
#include <Scene/SceneManager.h>
#include "LoadingThread.h"
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"
#include "Thread.h"

CGreen1LoadingScene::CGreen1LoadingScene()
{}

CGreen1LoadingScene::~CGreen1LoadingScene()
{
	SAFE_DELETE(m_LoadingThread);
}

bool CGreen1LoadingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_LoadingWidget = m_Scene->GetViewPort()->CreateUIWindow<CLoadingWidget>("LoadingWidget");

	m_LoadingWidget->SetTexture("Green1LoadingScene", TEXT("Project/Scene/Green1_LoadingScene.jpg"));

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("Loading");

	m_LoadingThread->SetLoadingSceneType(LoadingSceneType::Green1);

	// Loading Thread 내에 지정된 Run 함수를 호출할 것이다.
	m_LoadingThread->Start();

	return true;
}

void CGreen1LoadingScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

	if (!Queue->empty())
	{
		LoadingMessage Msg = Queue->front();

		Queue->pop();

		m_LoadingWidget->SetLoadingPercent(Msg.Percent);

		if (Msg.Complete)
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}

}
