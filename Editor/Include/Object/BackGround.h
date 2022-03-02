#pragma once

#include "GameObject\GameObject.h"
#include "Component/BackGroundComponent.h"

class CBackGround :
    public CGameObject
{
	friend class CScene;
private:
	CBackGround();
	CBackGround(const CBackGround& Map);
	virtual ~CBackGround();
private:
	CSharedPtr<CBackGroundComponent> m_BackGroundComponent;
public:

public:
	virtual void         Start();
	virtual bool         Init();
	virtual void         Update(float DeltaTime);
	virtual void         PostUpdate(float DeltaTime);
	virtual void         PrevRender();
	virtual void         Render();
	virtual void         PostRender();
	virtual CBackGround* Clone();
public:
	virtual void Save(FILE* pFile) override;
	virtual void Load(FILE* pFile) override;
};

