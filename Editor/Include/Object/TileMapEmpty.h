#pragma once

#include "GameObject\GameObject.h"
#include "Component/TileEmptyComponent.h"

class CTileMapEmpty :
    public CGameObject
{
	friend class CScene;
private:
	CTileMapEmpty();
	CTileMapEmpty(const CTileMapEmpty& Map);
	virtual ~CTileMapEmpty();
private:
	CSharedPtr<CTileEmptyComponent> m_TileEmptyComponent;
public:
	void SetEditMode(bool Enable)
	{
		m_TileEmptyComponent->SetEditMode(Enable);
	}
public:
	virtual void         Start();
	virtual bool         Init();
	virtual void         Update(float DeltaTime);
	virtual void         PostUpdate(float DeltaTime);
	virtual void         PrevRender();
	virtual void         Render();
	virtual void         PostRender();
	virtual CTileMapEmpty* Clone();
public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;
};

