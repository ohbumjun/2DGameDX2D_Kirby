#include "Dyna1LoadingScene.h"
#include "LoadingThread.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"


CDyna1LoadingScene::CDyna1LoadingScene()
{
	SetTypeID<CDyna1LoadingScene>();
}

CDyna1LoadingScene::~CDyna1LoadingScene()
{
	SAFE_DELETE(m_LoadingThread);
}

bool CDyna1LoadingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_LoadingWidget = m_Scene->GetViewPort()->CreateUIWindow<CLoadingWidget>("LoadingWidget");

	m_LoadingWidget->SetFrontTexture("Dyna1LoadingScene", TEXT("Project/Scene/DynaBlade_LoadingScene.png"));
	m_LoadingWidget->SetBackTexture("Dyna1LoadingSceneBack", TEXT("Project/Scene/DyanBlade_Back.png"));

	// 로딩 쓰레드 생성
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	m_LoadingThread->SetLoadingSceneType(LoadingSceneType::Dyna1);

	m_LoadingThread->Start();

	return true;
}

void CDyna1LoadingScene::Update(float DeltaTime)
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
