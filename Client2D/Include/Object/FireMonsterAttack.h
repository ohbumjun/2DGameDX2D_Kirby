#pragma once
#include "GameObject\GameObject.h"

class CFireMonsterAttack :
    public CGameObject
{
    friend class CFireMonster;
public:
    CFireMonsterAttack();
    CFireMonsterAttack(const CFireMonsterAttack& Attack);
    virtual ~CFireMonsterAttack() override;
private:
    class CFireMonster* m_FireMonsterOwner;
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    Vector2 m_AttackDir;
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
protected:
    void SetFireOwner(class CFireMonster* Owner)
    {
        m_FireMonsterOwner = Owner;
    }
protected:
    void SetRightAttackDir(float YDir);
    void SetLeftAttackDir(float YDir);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

