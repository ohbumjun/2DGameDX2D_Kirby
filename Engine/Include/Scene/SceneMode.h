#pragma once

#include "../Ref.h"

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

public:
	void SetPlayerObject(class CGameObject* Obj);
	void SetTileMapEmptyObject(class CGameObject* Obj);

	class CGameObject* GetPlayerObject() const
	{
		return m_PlayerObject;
	}

	class CGameObject* GetTileMapEmpty() const
	{
		return m_TileMapEmptyObject;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
};
