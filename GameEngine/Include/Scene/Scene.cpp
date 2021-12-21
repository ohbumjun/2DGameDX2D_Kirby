#include "Scene.h"

CScene::CScene()
{
	m_Mode     = new CSceneMode;
	m_Resource = new CSceneResource;

	m_Mode->m_Scene     = this;
	m_Resource->m_Scene = this;

	m_Start = false;
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
