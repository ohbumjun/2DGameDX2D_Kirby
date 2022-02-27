#pragma once
#include "GameObject\GameObject.h"
#include "Component/TileMapComponent.h"

class CTileMap :
    public CGameObject
{
    friend class CScene;
private :
    CTileMap();
    CTileMap(const CTileMap& Map);
    virtual ~CTileMap();
private :
    CSharedPtr<CTileMapComponent> m_TileComponent;
public:
	virtual void         Start();
	virtual bool         Init();
	virtual void         Update(float DeltaTime);
	virtual void         PostUpdate(float DeltaTime);
	virtual void         PrevRender();
	virtual void         Render();
	virtual void         PostRender();
	virtual CTileMap* Clone();
public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;
};

