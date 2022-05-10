#pragma once

#include "GameObject.h"
#include "../Pool/SimplePoolAllocator.h"

template<typename T>
class CGameObjectManager
{
	friend class CScene;
private :
	CGameObjectManager();
	virtual ~CGameObjectManager();
 private:
	// ��� Pool ���� Base Class ����
	// ��� ������ ��� �ְ� ����
	// allocate, deallocate �� virtual �� ����
	std::list <CSharedPtr<class CGameObject>> m_ObjectList;
	CSimplePoolAllocator<>* m_ObjectMemoryPool;
public :
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
};

template <typename T>
CGameObjectManager<T>::CGameObjectManager()
{}

template <typename T>
CGameObjectManager<T>::~CGameObjectManager()
{}

template <typename T>
void CGameObjectManager<T>::Update(float DeltaTime)
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			m_ObjectMemoryPool->deallocate((*iter));
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

template <typename T>
void CGameObjectManager<T>::PostUpdate(float DeltaTime)
{
	auto iter = m_ObjectList.begin();
	auto iterEnd = m_ObjectList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			m_ObjectMemoryPool->deallocate((*iter));
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

template <typename T>
void CGameObjectManager<T>::Render()
{
	// Object ��ϵ��� ���鼭 PrevRender ó�� --> RenderManager ��  m_RenderList ��Ͽ� �߰��Ѵ� -> ���� ���� Render �۾��� RenderManager ������
	auto iter = m_ObjectList->begin();
	auto iterEnd = m_ObjectList->end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PrevRender();
	}
}
