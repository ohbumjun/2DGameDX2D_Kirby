#pragma once
#include "AttackEffect.h"
#include "../Client.h"

class CKirbyAttackEffect :
    public CAttackEffect
{
    friend class CFireKirbyState;
    friend class CFightKirbyState;
    friend class CBeamKirbyState;
    friend class CBombKirbyState;
    friend class CSwordKirbyState;
public:
    CKirbyAttackEffect();
    CKirbyAttackEffect(const CKirbyAttackEffect& Attack);
    virtual ~CKirbyAttackEffect() override;
private:
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
    float m_AttackObjectSpeed;
    KirbyAttackEffect_Type m_AttackType;
    class CKirbyState* m_KirbyOwner;
    CSharedPtr<class CCameraComponent> m_Camera;
protected:
    void SetKirbyOwner(class CKirbyState* Owner)
    {
        m_KirbyOwner = Owner;
    }
    void SetAttackType(KirbyAttackEffect_Type Type);
    void ApplyCameraMove();
protected :
    virtual void BottomCollisionSpecificAction() override;
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
    void ApplyJumpEffect();
    void ApplyFallEffect();
    void ApplyBombFallAttackEnd();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

