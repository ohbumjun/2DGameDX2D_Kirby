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
    CSharedPtr<class CSpriteComponent> m_Sprite;
    float m_AttackDir;
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
protected:
    void SetFightOwner(class CFightMonster* Owner)
    {
        m_FightOwner = Owner;
    }
protected:
    void SetRightAttackDir();
    void SetLeftAttackDir();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};
