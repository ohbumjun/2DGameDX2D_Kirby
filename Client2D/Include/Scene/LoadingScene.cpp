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

	// Loading Thread ���� ������ Run �Լ��� ȣ���� ���̴�.
	m_LoadingThread->Start();

	return true;
}

void CLoadingScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	// ���⼭�� LoadingThread�� ThreadQueue�� �־��� Message�� ������
	// �۾��� ������ ���̴�. ��, LoaingThread��, ���� CLoadingScene�� ���� ����
	// Main Thread�� ThreadQueue ��� �ڿ��� �����ϸ鼭,
	// ������ push��, ������ pop�� �ϰ� �ִ�.
	// ������, ������ push�Ҷ��� pop�� �ϸ� �ȵȴ�
	// �̸� ���ؼ� ThreadQueue ���� CSync ��ü��, ���� �Լ� �ȿ� �־
	// Critical Section�� ������ ���̴�.
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
