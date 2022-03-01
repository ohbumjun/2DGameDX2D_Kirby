#pragma once
#include "GameObject.h"
class CLifeObject :
    public CGameObject
{
protected:
	CLifeObject();
	CLifeObject(const CLifeObject& obj);
	virtual ~CLifeObject() override;
public:
	virtual void         Start() override;
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime) override;
	virtual void         PostUpdate(float DeltaTime) override;
	virtual void         PrevRender() override;
	virtual void         Render() override;
	virtual void         PostRender() override;
	virtual CLifeObject* Clone() override;
};

