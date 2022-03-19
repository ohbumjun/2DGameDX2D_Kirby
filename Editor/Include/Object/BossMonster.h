#pragma once
#include "Object\Monster.h"

class CBossMonster :
    public CMonster
{
    friend class CScene;
protected:
    CBossMonster();
    CBossMonster(const CBossMonster& Monster);
    virtual ~CBossMonster();
protected:
    virtual void AIDeathSpecific(float DeltaTime) {}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossMonster* Clone() override;
};

