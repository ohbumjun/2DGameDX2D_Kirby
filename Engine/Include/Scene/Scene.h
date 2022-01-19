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
	CSharedPtr<CSceneMode>             m_Mode;
	CSceneResource*                    m_Resource;
	class CCameraManager* m_CameraManager;
	std::list<CSharedPtr<CGameObject>> m_ObjList;
	class CSceneCollision* m_Collision;
	class CViewPort* m_ViewPort;
	bool                               m_Start;

public:
	class CViewPort* GetViewPort() const
	{
		return m_ViewPort;
	}
	class CCameraManager* GetCameraManager() const
	{
		return m_CameraManager;
	}
	class CSceneCollision* GetCollision() const
	{
		return m_Collision;
	}
	CSceneResource* GetResource() const 
	{
		return m_Resource;
	}

	CGameObject* GetPlayerObject() const
	{
		return m_Mode->GetPlayerObject();
	}
	CGameObject* FindGameObject(const char* ObjectName) const;
public :
	void GatherObjectsNames(std::vector<std::string>& vecObjNames);
public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
public :
	void SaveFullPath(const char* FullPath);
	void LoadFullPath(const char* FullPath);
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
	template<typename T>
	void LoadSceneMode()
	{
		m_Mode = new T;
		m_Mode->m_Scene = this;
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
	template<typename T>
	T* LoadGameObject()
	{
		T* Object = new T;
		Object->SetScene(this);

		m_ObjList.push_back(Object);

		if (m_Start)
			Object->Start();

		return Object;
	}

};
