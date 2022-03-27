#include "Float1LoadingScene.h"
#include "LoadingThread.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"


CFloat1LoadingScene::CFloat1LoadingScene()
{
	SetTypeID<CFloat1LoadingScene>();
}

CFloat1LoadingScene::~CFloat1LoadingScene()
{
	SAFE_DELETE(m_LoadingThread);
}

bool CFloat1LoadingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_LoadingWidget = m_Scene->GetViewPort()->CreateUIWindow<CLoadingWidget>("LoadingWidget");

	m_LoadingWidget->SetFrontTexture("Float1LoadingScene", TEXT("Project/Scene/Float1_LoadingScene.png"));
	m_LoadingWidget->SetBackTexture("Float1LoadingSceneBack", TEXT("Project/Scene/FloatIsland1_Back.png"));

	// 로딩 쓰레드 생성
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	m_LoadingThread->SetLoadingSceneType(LoadingSceneType::Float1);

	m_LoadingThread->Start();

	return true;
}

void CFloat1LoadingScene::Update(float DeltaTime)
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
