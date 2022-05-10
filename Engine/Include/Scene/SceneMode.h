#pragma once

#include "../Ref.h"
#include "../GameObject/GameObject.h"

class CSceneMode :
	public CRef
{
	friend class CScene;

protected:
	CSceneMode();
	virtual ~CSceneMode() override;

protected:
	class CScene*                 m_Scene;
	CSharedPtr<class CGameObject> m_PlayerObject;
	CSharedPtr<class CGameObject> m_TileMapEmptyObject;
	CSharedPtr<class CGameObject> m_LineContainerObject;
	CSharedPtr<class CGameObject> m_SceneChangeObject;
	CSharedPtr<class CGameObject> m_BossMonsterObject;
protected:
	bool m_IsEditMode;
	int m_SceneIndex;
public:
	void SetPlayerObject(class CGameObject* Obj);
	void SetTileMapEmptyObject(class CGameObject* Obj);
	void SetLineContainerObject(class CGameObject* Obj);
	void SetSceneChangeObject(class CGameObject* Obj);
	void SetBossObject(class CGameObject* Obj);
protected :
	virtual void CreateMaterial();
	virtual void CreateParticle();
public :
	void SetIsEditMode (bool Enable)
{
		m_Enable = Enable;
}
public :
	int GetSceneIndex() const
{
		return m_SceneIndex;
}
	bool IsEditMode () const
{
		return m_IsEditMode;
}
	class CGameObject* GetPlayerObject() const
	{
		return m_PlayerObject;
	}
	class CGameObject* GetBossMonsterObject() const
	{
		return m_BossMonsterObject;
	}
	class CGameObject* GetTileMapEmpty() const
	{
		return m_TileMapEmptyObject;
	}
	class CGameObject* GetLineContainer() const
	{
		return m_LineContainerObject;
	}
	class CGameObject* GetSceneChangeObject() const
	{
		return m_SceneChangeObject;
	}

	class CTileEmptyComponent* GetTileEmptyComponent () const
	{
		if (!m_TileMapEmptyObject)
			return nullptr;
		return (CTileEmptyComponent*)m_TileMapEmptyObject->GetRootComponent();
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
public :
	virtual void PrepareResources();
	virtual void CreateSound();
};
