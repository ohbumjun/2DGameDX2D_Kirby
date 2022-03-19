#pragma once
#include "AttackEffect.h"

class CFireMonsterAttack :
    public CAttackEffect
{
    friend class CFireMonster;
public:
    CFireMonsterAttack();
    CFireMonsterAttack(const CFireMonsterAttack& Attack);
    virtual ~CFireMonsterAttack() override;
private:
    class CFireMonster* m_FireMonsterOwner;
private:
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
protected:
    void SetFireOwner(class CFireMonster* Owner)
    {
        m_FireMonsterOwner = Owner;
    }
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

