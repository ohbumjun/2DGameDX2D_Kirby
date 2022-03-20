#pragma once
#include "AbilityMonster.h"

class CFireMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CFireMonster();
    CFireMonster(const CFireMonster& Monster);
    virtual ~CFireMonster();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CFireMonster* Clone() override;
private:
    virtual void Attack() override;
    virtual void AITraceSpecific(float DeltaTime) override;
};

