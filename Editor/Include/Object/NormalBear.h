#pragma once
#include "Object/NormalMonster.h"

class CNormalBear :
    public CNormalMonster
{
    friend class CScene;
protected:
	CNormalBear();
	virtual ~CNormalBear() override;
public:
	virtual void         Start() override;
	virtual bool         Init() override;
	virtual void         Update(float DeltaTime)override;
	virtual void         PostUpdate(float DeltaTime)override;
	virtual CNormalBear* Clone() override;
};

