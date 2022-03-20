#pragma once
#include "AttackEffect.h"

class CHammerGorillaCloseAttack :
    public CAttackEffect
{
    friend class CMiddleBossHammer;
public:
    CHammerGorillaCloseAttack();
    CHammerGorillaCloseAttack(const CHammerGorillaCloseAttack& Attack);
    virtual ~CHammerGorillaCloseAttack() override;
private:
    class CMiddleBossHammer* m_MiddleBossHammer;
protected:
    void SetFireOwner(class CMiddleBossHammer* Owner)
    {
        m_MiddleBossHammer = Owner;
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
