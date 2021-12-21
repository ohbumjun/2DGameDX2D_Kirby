#pragma once

#include "Component.h"

class CObjectComponent :
	public CComponent
{
	friend class CGameObject;

protected:
	CObjectComponent();
	CObjectComponent(const CObjectComponent& com);
	virtual ~CObjectComponent() override = 0;

public:
	virtual void              Start() override;
	virtual bool              Init() override = 0;
	virtual void              Update(float DeltaTime) override = 0;
	virtual void              PostUpdate(float DeltaTime) override = 0;
	virtual void              PrevRender() override = 0;
	virtual void              Render() override = 0;
	virtual void              PostRender() override = 0;
	virtual CObjectComponent* Clone() override = 0;
};
