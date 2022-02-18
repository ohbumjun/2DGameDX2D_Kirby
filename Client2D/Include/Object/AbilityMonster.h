#pragma once

#include "Monster.h"

class CAbilityMonster :
    public CMonster
{
    friend class CScene;
protected:
    CAbilityMonster();
    CAbilityMonster(const CAbilityMonster& Monster);
    virtual ~CAbilityMonster();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAbilityMonster* Clone() override;
};

