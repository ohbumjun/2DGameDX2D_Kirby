#include "GameObjectManager.h"

CGameObjectManager::CGameObjectManager()
{}

CGameObjectManager::~CGameObjectManager()
{
	// Object 목록들을 돌면서 PrevRender 처리 --> RenderManager 의  m_RenderList 목록에 추가한다 -> 이후 실제 Render 작업은 RenderManager 측에서
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
	// Object 목록들을 돌면서 PrevRender 처리 --> RenderManager 의  m_RenderList 목록에 추가한다 -> 이후 실제 Render 작업은 RenderManager 측에서
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
