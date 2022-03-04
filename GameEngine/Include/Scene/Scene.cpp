#include "Scene.h"
#include "SceneCollision.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ViewPort.h"
#include "NavigationManager.h"
#include "../PathManager.h"

CScene::CScene()  :
	m_Change(false)
{
	m_Mode     = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_ViewPort = new CViewPort;
	m_NavManager = new CNavigationManager;

	m_Mode->m_Scene     = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_ViewPort->m_Scene = this;
	m_NavManager->m_Scene = this;
	
	m_Start = false;
	m_Collision->Init();
	m_CameraManager->Init();
	m_ViewPort->Init();
	m_NavManager->Init();

	Resolution RS = CEngine::GetInst()->GetResolution();

	m_WorldResolution = Vector2((float)RS.Width, (float)RS.Height);
}

CScene::~CScene()
{
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_ViewPort);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_NavManager);
}

void CScene::Start()
{
	m_Mode->Start();

	auto iter    = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_Collision->Start();
	m_CameraManager->Start();
	m_ViewPort->Start();
	m_NavManager->Start();
	m_Start = true;

	// 카메라 Component가 세팅되어 있다면
	// 카메라 매니저의 메인 카메라로 세팅해둔다
	if (m_Mode->GetPlayerObject())
	{
		CCameraComponent* Camera = m_Mode->GetPlayerObject()->FindComponentByType<CCameraComponent>();

		if (Camera)
		{
			m_CameraManager->SetCurrentCamera(Camera);
		}
	}
}

void CScene::Update(float DeltaTime)
{
	m_Mode->Update(DeltaTime);

	auto iter    = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter    = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_ViewPort->Update(DeltaTime);
	m_CameraManager->Update(DeltaTime);
	m_NavManager->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	m_Mode->PostUpdate(DeltaTime);

	auto iter    = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter    = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	// 충돌체들을 충돌 영역에 포함시킨다
	iter	   = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}

	// 포함된 충돌체들을 이용해서 충돌처리를 진행한다.
	m_Collision->Collision(DeltaTime);

	m_ViewPort->PostUpdate(DeltaTime);

	m_CameraManager->PostUpdate(DeltaTime);
}

void CScene::SaveFullPath(const char* FullPath)
{
	/*
	CSceneResource*                    m_Resource; --> 왜 저장안해주는 것일까 ??
	std::list<CSharedPtr<CGameObject>> m_ObjList;
	bool                               m_Start;
	 */
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "wb");
	if (!pFile)
		return;

	size_t TypeID = m_Mode->GetTypeID();
	fwrite(&TypeID, sizeof(size_t), 1, pFile);

	// SceneEdit Object가 있을 경우, 저장을 하면 안된다.
	// DeleteGameObject("SceneEditObject");

	fwrite(&m_WorldResolution, sizeof(Vector2), 1, pFile);

	size_t ObjectCount = m_ObjList.size();

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		// if ((*iter)->CheckType<CShowObject>())
		if ((*iter)->GetName() == OBJECTNAME_SHOWOBJECT)
		{
			ObjectCount -= 1;
			break;
		}
	}

	fwrite(&ObjectCount, sizeof(size_t), 1, pFile);

	iter = m_ObjList.begin();

	for (;iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == "SceneEditObject")
			continue;
		size_t TypeID = (*iter)->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, pFile);
		(*iter)->Save(pFile);
	}

	fclose(pFile);
}

bool CScene::LoadFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return false;

	m_ObjList.clear();

	size_t TypeID;
	fread(&TypeID, sizeof(size_t), 1, pFile);
	CSceneManager::GetInst()->CallCreateSceneModeFunc(this, TypeID);

	fread(&m_WorldResolution, sizeof(Vector2), 1, pFile);

	size_t ObjectCount;
	fread(&ObjectCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < ObjectCount; i++)
	{
		fread(&TypeID, sizeof(size_t), 1, pFile);

		CGameObject* Object = CSceneManager::GetInst()->CallCreateObjectFunc(this, TypeID);

		if (!Object)
			return false;

		Object->Load(pFile);

		// ex) 충돌체 목록들을 SceneCollision List 에 추가하기 
		if (m_Start)
			Object->Start();

		// Object->SetScene(this);
	}

	fclose(pFile);

	return true;
}

void CScene::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Path = CPathManager::GetInst()->FindPath(PathName);

	char FullPath[MAX_PATH] = {};

	if (Path)
		strcpy_s(FullPath, Path->PathMultibyte);

	strcat_s(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CScene::ClearGameObjects()
{
	m_ObjList.clear();
}

void CScene::DeleteGameObject(const std::string& Name)
{
	auto iter      = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			m_ObjList.erase(iter);
			break;
		}
	}
}

void CScene::RemoveDuplicateObject(const std::string& Name)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	auto iterFound = iter;

	// 하나를 찾아내고
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			iterFound = ++iter;
			break;
		}
	}

	// 이후 부터를 검사해서 중복 제거
	for (; iterFound != iterEnd;)
	{
		if ((*iterFound)->GetName() == Name)
		{
			iterFound = m_ObjList.erase(iterFound);
			iterEnd = m_ObjList.end();
			continue;
		}
		++iterFound;
	}
}

CGameObject* CScene::FindGameObject(const char* ObjectName) const
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == ObjectName)
			return (*iter);
	}
	return nullptr;
}

CGameObject* CScene::FindGameObjectByTypeID(size_t TypeID) const
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_TypeID == TypeID)
			return (*iter);
	}
	return nullptr;
}


void CScene::GatherObjectsNames(std::vector<std::string>& vecObjNames)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		vecObjNames.push_back((*iter)->GetName());
	}
}

void CScene::SetAutoChange(bool Change)
{
	m_Change = Change;
}
