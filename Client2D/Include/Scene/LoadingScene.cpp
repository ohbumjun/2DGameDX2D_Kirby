#include "LoadingScene.h"

#include <Scene/SceneManager.h>

#include "LoadingThread.h"
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"
#include "Thread.h"

CLoadingScene::CLoadingScene()
{}

CLoadingScene::~CLoadingScene()
{
	SAFE_DELETE(m_LoadingThread);
}

bool CLoadingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_LoadingWidget = m_Scene->GetViewPort()->CreateUIWindow<CLoadingWidget>("LoadingWidget");

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("Loading");

	// Loading Thread 내에 지정된 Run 함수를 호출할 것이다.
	m_LoadingThread->Start();

	return true;
}

void CLoadingScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	// 여기서는 LoadingThread가 ThreadQueue에 넣어준 Message를 꺼내서
	// 작업을 진행할 것이다. 단, LoaingThread와, 현재 CLoadingScene을 진행 중인
	// Main Thread는 ThreadQueue 라는 자원을 공유하면서,
	// 한쪽은 push를, 한쪽은 pop을 하고 있다.
	// 하지만, 한쪽이 push할때는 pop을 하면 안된다
	// 이를 위해서 ThreadQueue 내에 CSync 객체를, 각각 함수 안에 넣어서
	// Critical Section을 구현한 것이다.
	CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

	if (!Queue->empty())
	{
		LoadingMessage Msg = Queue->front();
		Queue->pop();

		m_LoadingWidget->SetLoadingPercent(Msg.Percent);

		if (Msg.Complete)
			CSceneManager::GetInst()->ChangeNextScene();
	}

}
