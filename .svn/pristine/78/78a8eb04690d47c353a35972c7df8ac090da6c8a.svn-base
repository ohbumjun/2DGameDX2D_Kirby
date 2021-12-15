#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "../GameObject/GameObject.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	CSharedPtr<CSceneMode> m_Mode;
	CSceneResource* m_Resource;
	std::list<CSharedPtr<CGameObject>>	m_ObjList;
	bool		m_Start;

public:
	CSceneResource* GetResource()	const
	{
		return m_Resource;
	}

	CGameObject* GetPlayerObject()	const
	{
		return m_Mode->GetPlayerObject();
	}

public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);

public:
	template <typename T>
	bool CreateSceneMode()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		if (!m_Mode->Init())
		{
			m_Mode = nullptr;
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateGameObject(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetName(Name);
		Obj->SetScene(this);

		if (!Obj->Init())
		{
			SAFE_RELEASE(Obj);
			return nullptr;
		}

		m_ObjList.push_back(Obj);

		if (m_Start)
			Obj->Start();

		return Obj;
	}
};

