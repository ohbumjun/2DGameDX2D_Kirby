#include "SceneManager.h"

#include "CameraManager.h"
#include "../Render/RenderManager.h"
#include "../Sync.h"
#include "../Component/ColliderComponent.h"

DEFINITION_SINGLE(CSceneManager)

CSharedPtr<CGameObject> CSceneManager:: m_StaticPlayer = nullptr;

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr)
{
	InitializeCriticalSection(&m_Crt);
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_Scene);
	SAFE_DELETE(m_NextScene);

	DeleteCriticalSection(&m_Crt);
}

void CSceneManager::Start()
{
	m_Scene->Start();
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::ChangeScene()
{
	// ex) CreateNewScene �ϰ� �ִ� ���ȿ��� ChangeScene�� �������� �ʰ� �Ѵ�
	CSync sync(&m_Crt);

	if (m_NextScene)
	{
		if (m_NextScene->m_Change)
		{
			m_StaticPlayer = m_Scene->GetPlayerObject();

			m_NextScene->GetCameraManager()->SetCurrentCamera(m_Scene->GetCameraManager()->GetCurrentCamera());

			// ������� Delay�� �ش�.
			Sleep(1000);

			// Scene ���� ��ȭ
			SAFE_DELETE(m_Scene);

			m_Scene = m_NextScene;

			if (m_StaticPlayer)
			{
				m_Scene->SetPlayerObject(m_StaticPlayer);

				m_StaticPlayer->SetScene(m_Scene);

				// ���� Scene ���� Player2D �� �ش��ϴ� GameObject�� �����.
				// m_Scene->DeletePlayerFromScene();

				// �ش� m_StaticPlayer�� Obj List�� �߰��Ѵ�. 
				m_Scene->m_ObjList.push_back(m_StaticPlayer);

				m_Scene->m_PlayerTypeID = m_StaticPlayer->GetTypeID();
			}

			m_NextScene = nullptr;

			// Object ��� ���� 
			CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

			// Start �Լ� ȣ��
			m_Scene->Start();


			return true;
		}
	}

	return false;
}


void CSceneManager::CreateNewScene(bool AutoChange)
{
	CSync sync(&m_Crt);

	m_NextScene = new CScene;

	m_NextScene->SetAutoChange(AutoChange);
}
