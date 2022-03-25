#pragma once
#include "AttackEffect.h"

class CPenguinCloseAttack :
    public CAttackEffect
{
    friend class CBossPenguin;
public:
    CPenguinCloseAttack();
    CPenguinCloseAttack(const CPenguinCloseAttack& Attack);
    virtual ~CPenguinCloseAttack() override;
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

