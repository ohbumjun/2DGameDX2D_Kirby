#pragma once
#include "Monster.h"

class CWaterMonster :
    public CMonster
{
    friend class CScene;
protected:
    CWaterMonster();
    CWaterMonster(const CWaterMonster& Monster); //
    virtual ~CWaterMonster();
protected:
    void UpdateAttackLimitTime(float DeltaTime);
    virtual void Attack();
    virtual void ChangeAttackAnimation() override;
    virtual void AIDeathSpecific(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CWaterMonster* Clone() override;
};

