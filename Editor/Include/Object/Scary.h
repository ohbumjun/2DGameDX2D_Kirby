#pragma once
#include "NormalMonster.h"

class CScary :
    public CNormalMonster
{
	friend class CScene;
protected:
	CScary();
	virtual ~CScary() override;
protected:
	virtual void Damage(float Damage) override {}
public:
	virtual void         Start() override;
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime)override;
	virtual void         PostUpdate(float DeltaTime)override;
};

