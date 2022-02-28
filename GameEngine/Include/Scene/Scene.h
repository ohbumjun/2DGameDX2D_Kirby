#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "../GameObject/GameObject.h"

class CScene
{
	friend class CSceneManager;
	friend class CEditorMenu;

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
	class CNavigationManager* m_NavManager;
	bool                               m_Start;
	bool m_Change;
public :
	void SetPlayerObject(CGameObject* Object)
{
		m_Mode->SetPlayerObject(Object);
}
public:
	class CNavigationManager* GetNavManager() const
	{
		return m_NavManager;
	}
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
private :
	void ClearGameObjects();
	void DeleteGameObject(const std::string& Name);
	void RemoveDuplicateObject(const std::string& Name);
public :
	CGameObject* FindGameObject(const char* ObjectName) const;
	void GatherObjectsNames(std::vector<std::string>& vecObjNames);
	void SetAutoChange(bool Change);
public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
public :
	void SaveFullPath(const char* FullPath);
	bool LoadFullPath(const char* FullPath);
public:
	template<typename T>
	T* CreateSceneModeEmpty()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return m_Mode;
	}

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
