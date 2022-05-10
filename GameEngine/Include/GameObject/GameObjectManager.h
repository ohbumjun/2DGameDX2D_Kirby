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
	// 모든 Pool 들의 Base Class 제작
	// 멤버 변수로 들고 있게 세팅
	// allocate, deallocate 를 virtual 로 세팅
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
	// Object 목록들을 돌면서 PrevRender 처리 --> RenderManager 의  m_RenderList 목록에 추가한다 -> 이후 실제 Render 작업은 RenderManager 측에서
	auto iter = m_ObjectList->begin();
	auto iterEnd = m_ObjectList->end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->PrevRender();
	}
}
