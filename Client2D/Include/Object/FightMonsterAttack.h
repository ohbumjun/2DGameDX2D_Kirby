#pragma once
#include "AttackEffect.h"

class CFightMonsterAttack :
    public CAttackEffect
{
    friend class CFightMonster;
public:
    CFightMonsterAttack();
    CFightMonsterAttack(const CFightMonsterAttack& Attack);
    virtual ~CFightMonsterAttack() override;
private:
    class CFightMonster* m_FightOwner;
private:
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
protected:
    void SetFightOwner(class CFightMonster* Owner)
    {
        m_FightOwner = Owner;
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

