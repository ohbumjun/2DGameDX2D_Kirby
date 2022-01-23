#include "SceneManager.h"
#include "../Render/RenderManager.h"
#include "../Sync.h"

DEFINITION_SINGLE(CSceneManager)

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

void CSceneManager::CreateNewScene(bool AutoChange)
{
	CSync sync(&m_Crt);

	m_NextScene = new CScene;

	m_NextScene->SetAutoChange(AutoChange);
}
