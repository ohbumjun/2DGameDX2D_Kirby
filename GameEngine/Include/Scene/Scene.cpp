#include "Scene.h"
#include "SceneCollision.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ViewPort.h"

CScene::CScene()
{
	m_Mode     = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_ViewPort = new CViewPort;

	m_Mode->m_Scene     = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_ViewPort->m_Scene = this;
	
	m_Start = false;
	m_Collision->Init();
	m_CameraManager->Init();
	m_ViewPort->Init();
}

CScene::~CScene()
{
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_ViewPort);
	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_Collision);
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

	// 포함된 충돌체들을 이용해서 충돌처리를 진행한다
	m_Collision->Collision(DeltaTime);

	m_ViewPort->PostUpdate(DeltaTime);
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

	size_t ObjectCount = m_ObjList.size();
	fwrite(&ObjectCount, sizeof(size_t), 1, pFile);

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (;iter != iterEnd; ++iter)
	{
		size_t TypeID = (*iter)->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, pFile);
		(*iter)->Save(pFile);
	}

	fclose(pFile);
}

void CScene::LoadFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return;

	m_ObjList.clear();

	size_t TypeID;
	fread(&TypeID, sizeof(size_t), 1, pFile);
	CSceneManager::GetInst()->CallCreateSceneModeFunc(this, TypeID);

	size_t ObjectCount;
	fread(&ObjectCount, sizeof(size_t), 1, pFile);

	for (size_t i = 0; i < ObjectCount; i++)
	{
		fread(&TypeID, sizeof(size_t), 1, pFile);
		CGameObject* Object = CSceneManager::GetInst()->CallCreateObjectFunc(this, TypeID);
		Object->Load(pFile);
	}

	fclose(pFile);
}

void CScene::ClearGameObjects()
{
	m_ObjList.clear();
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


void CScene::GatherObjectsNames(std::vector<std::string>& vecObjNames)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		vecObjNames.push_back((*iter)->GetName());
	}
}
