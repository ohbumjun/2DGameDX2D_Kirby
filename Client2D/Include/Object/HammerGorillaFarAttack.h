#pragma once
#include "AttackEffect.h"

class CHammerGorillaFarAttack :
    public CAttackEffect
{
    friend class CMiddleBossHammer;
public:
    CHammerGorillaFarAttack();
    CHammerGorillaFarAttack(const CHammerGorillaFarAttack& Attack);
    virtual ~CHammerGorillaFarAttack() override;
private:
    class CMiddleBossHammer* m_MiddleBossHammer;
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    Vector2 m_AttackDir;
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
    bool m_CreateMultipleAfter;
protected:
    void SetFireOwner(class CMiddleBossHammer* Owner)
    {
        m_MiddleBossHammer = Owner;
    }
    void SetCreateMultileAfter(bool Enable)
    {
        m_CreateMultipleAfter = Enable;
    }
protected:
    void SetRightAttackDir(float YDir);
    void SetLeftAttackDir(float YDir);
    void MakeMultipleAttackEffect();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

