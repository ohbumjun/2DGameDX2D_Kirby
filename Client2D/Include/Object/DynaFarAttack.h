#pragma once
#include "AttackEffect.h"

class CDynaFarAttack :
    public CAttackEffect
{
    friend class CBossDyna;
public:
    CDynaFarAttack();
    virtual ~CDynaFarAttack() override;
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
private:
    virtual void BottomCollisionSpecificAction() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

