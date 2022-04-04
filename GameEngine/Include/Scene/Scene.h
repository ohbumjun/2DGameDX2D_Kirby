#pragma once

#include "SceneMode.h"
#include "SceneResource.h"
#include "ViewPort.h"
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
	size_t m_PlayerTypeID;
	
private :
	Vector2 m_BossWorldResolution;
	Vector2 m_WorldResolution;
public :
	void SetBossWorldResolution(float x, float y)
{
		m_BossWorldResolution = Vector2(x, y);
}
	void SetPlayerTypeID(size_t TypeID)
{
		m_PlayerTypeID = TypeID;
}
	void SetWorldResolution (float Width, float Height)
{
		m_WorldResolution = Vector2(Width, Height);
}
	void SetPlayerObject(CGameObject* Object)
{
		m_Mode->SetPlayerObject(Object);
}
	void SetSceneChangeObject(CGameObject* Object)
{
		m_Mode->SetSceneChangeObject(Object);
}
	void SetIsEditMode (bool Enable)
	{
		m_Mode->SetIsEditMode(Enable);
	}
	void SetPlayerHUD(class CUIWindow* Window)
	{
		m_ViewPort->SetPlayerHUD(Window);
	}
	void SetBossHUD(class CUIWindow* Window)
	{
		m_ViewPort->SetBossHUD(Window);
	}
public:
	bool IsEditMode () const
	{
		return m_Mode->IsEditMode();
	}
	class CUIWindow* GetPlayerHUD() const
	{
		return m_ViewPort->GetPlayerHUD();
	}
	class CUIWindow* GetBossHUD() const
	{
		return m_ViewPort->GetBossHUD();
	}
	CSceneMode* GetSceneMode() const
	{
		return m_Mode;
	}
	Vector2 GetBossWorldResolution() const
	{
		return m_BossWorldResolution;
	}
	Vector2 GetWorldResolution() const
	{
		return m_WorldResolution;
	}
	std::list<CSharedPtr<CGameObject>> GetObjectLists() const
	{
		return m_ObjList;
	}
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
	CGameObject* GetTileMapEmptyObject() const
	{
		return m_Mode->GetTileMapEmpty();
	}
	CGameObject* GetLineContainerObject() const
	{
		return m_Mode->GetLineContainer();
	}
	CGameObject* GetSceneChangeObject() const
	{
		return m_Mode->GetSceneChangeObject();
	}
	class CTileEmptyComponent* GetTileEmptyComponent() const
	{
		return m_Mode->GetTileEmptyComponent();
	}
public :
	void PrepareResources()
	{
		m_Mode->PrepareResources();
	}
	template<typename T>
	void DeleteGameObjectByType();
	void DeleteGameObject(const std::string& Name);
private :
	void ClearGameObjects();
	void DeletePlayerFromScene();
	void RemoveDuplicateObject(const std::string& Name);
public :
	void SetStopEnableObjectsExceptPlayer(size_t TypeID, bool Enable);
public :
	CGameObject* FindGameObject(const char* ObjectName) const;
	CGameObject* FindGameObjectByTypeID(size_t TypeID) const;
	void SetAutoChange(bool Change);
	void GatherObjectsNames(std::vector<std::string>& vecObjNames);
	template<typename T>
	void GatherSpecificTypeObjectsName(std::vector<std::string>& vecObjNames);
public:
	void Start();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
public :
	void SaveFullPath(const char* FullPath);
	bool LoadFullPath(const char* FullPath);
	bool Load(const char* FileName, const std::string& PathName);
public:
	template<typename T>
	T* CreateSceneModeEmpty()
	{
		m_Mode = new T;

		m_Mode->m_Scene = this;

		return (T*)*m_Mode;
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
	T* LoadSceneMode()
	{
		m_Mode = new T;
		m_Mode->m_Scene = this;

		return (T*)*m_Mode;
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

template <typename T>
void CScene::DeleteGameObjectByType()
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CheckType<T>())
		{
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

template <typename T>
void CScene::GatherSpecificTypeObjectsName(std::vector<std::string>& vecObjNames)
{
	vecObjNames.reserve(m_ObjList.size());

	auto iter      = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->CheckType<T>())
		{
			vecObjNames.push_back((*iter)->GetName());
		}
	}
}
