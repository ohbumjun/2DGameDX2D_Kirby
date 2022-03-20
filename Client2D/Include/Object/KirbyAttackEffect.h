#pragma once
#include "AttackEffect.h"
#include "../Client.h"

class CKirbyAttackEffect :
    public CAttackEffect
{
    friend class CFireKirbyState;
    friend class CFightKirbyState;
    friend class CBeamKirbyState;
public:
    CKirbyAttackEffect();
    CKirbyAttackEffect(const CKirbyAttackEffect& Attack);
    virtual ~CKirbyAttackEffect() override;
private:
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
    float m_AttackObjectSpeed;
    KirbyAttackEffect_Type m_AttackType;
protected:
    void SetAttackType(KirbyAttackEffect_Type Type);
protected :
    virtual void BottomCollisionSpecificAction() override;
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

