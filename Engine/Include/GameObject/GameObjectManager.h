#pragma once

#include "GameObject.h"
// #include "../Pool/SimplePoolAllocator.h"

// 해당 Class 를 만드는 이유 --> Scene 내의 Object List 에 추가하지 않은 상태에서
// Update 및 Render 를 하기 위해서
class CGameObjectManager
{
	friend class CScene;
protected: 
	CGameObjectManager();
	virtual ~CGameObjectManager();
protected:
	// 모든 Pool 들의 Base Class 제작
	// 멤버 변수로 들고 있게 세팅
	// allocate, deallocate 를 virtual 로 세팅
	std::list <class CGameObject*> m_ObjectList;
	std::function<void(class CGameObject*)> m_DeleteCallback;
	// CSimplePoolAllocator<>* m_ObjectMemoryPool;
	
	template<typename T>
	void SetDeleteCallback(T* Obj, void(T::* Func)(class CGameObject* Object))
	{
		m_DeleteCallback = std::bind(Func, Obj, std::placeholders::_1);
	}
public :
	virtual void Update(float DeltaTime) = 0;
	virtual void PostUpdate(float DeltaTime) = 0;
	void PrevRender();
	void AddCollision();
	virtual void DeAllocate(class CGameObject* m_CurrentDynaBaby) = 0;
};
