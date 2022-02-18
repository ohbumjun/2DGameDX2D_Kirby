#pragma once
#include "Monster.h"

class CNormalMonster :
    public CMonster
{
    friend class CScene;
protected :
    CNormalMonster();
    CNormalMonster(const CNormalMonster& Monster);
    virtual ~CNormalMonster();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CNormalMonster* Clone() override;
};

