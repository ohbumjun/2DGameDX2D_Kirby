#pragma once
#include "AttackEffect.h"

class CEffectWaterAttack :
    public CAttackEffect
{
    friend class CPlayer2D;
public:
    CEffectWaterAttack();
    CEffectWaterAttack(const CEffectWaterAttack& Attack);
    virtual ~CEffectWaterAttack() override;
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

