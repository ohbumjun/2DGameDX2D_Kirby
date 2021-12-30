#include "Scene.h"
#include "SceneCollision.h"
#include "SceneManager.h"

CScene::CScene()
{
	m_Mode     = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;

	m_Mode->m_Scene     = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	
	m_Start = false;
	m_Collision->Init();
}

CScene::~CScene()
{
	SAFE_DELETE(m_Resource);
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

	m_Start = true;
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
