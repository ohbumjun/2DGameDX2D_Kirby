#pragma once
#include "AttackEffect.h"

class CSwordMonsterAttack :
    public CAttackEffect
{
    friend class CSwordMonster;
public:
    CSwordMonsterAttack();
    virtual ~CSwordMonsterAttack() override;
private:
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

