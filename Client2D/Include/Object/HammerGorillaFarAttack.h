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
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
    bool m_CreateMultipleAfter;
protected:
    void SetCreateMultipleEffectAfterWards(bool Enable)
    {
        m_CreateMultipleAfter = Enable;
    }
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
    void MakeMultipleAttackEffect();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

