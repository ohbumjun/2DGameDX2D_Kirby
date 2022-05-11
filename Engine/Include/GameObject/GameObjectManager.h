#pragma once

#include "GameObject.h"
// #include "../Pool/SimplePoolAllocator.h"

// �ش� Class �� ����� ���� --> Scene ���� Object List �� �߰����� ���� ���¿���
// Update �� Render �� �ϱ� ���ؼ�
class CGameObjectManager
{
	friend class CScene;
protected: 
	CGameObjectManager();
	virtual ~CGameObjectManager();
protected:
	// ��� Pool ���� Base Class ����
	// ��� ������ ��� �ְ� ����
	// allocate, deallocate �� virtual �� ����
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
