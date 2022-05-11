#include "GameObjectManager.h"

CGameObjectManager::CGameObjectManager()
{}

CGameObjectManager::~CGameObjectManager()
{
	// Object ��ϵ��� ���鼭 PrevRender ó�� --> RenderManager ��  m_RenderList ��Ͽ� �߰��Ѵ� -> ���� ���� Render �۾��� RenderManager ������
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd; ++iter)
	{
		m_DeleteCallback(*iter);
	}
}

void CGameObjectManager::Update(float DeltaTime)
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			m_DeleteCallback(*iter);
			iter = m_ObjectList.erase(iter);
			iterEnd = m_ObjectList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Update(DeltaTime);
		++iter;
	}
}

void CGameObjectManager::PostUpdate(float DeltaTime)
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			m_DeleteCallback(*iter);
			iter = m_ObjectList.erase(iter);
			iterEnd = m_ObjectList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

void CGameObjectManager::PrevRender()
{
	// Object ��ϵ��� ���鼭 PrevRender ó�� --> RenderManager ��  m_RenderList ��Ͽ� �߰��Ѵ� -> ���� ���� Render �۾��� RenderManager ������
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PrevRender();
	}
}

void CGameObjectManager::AddCollision()
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->AddCollision();
	}
}
