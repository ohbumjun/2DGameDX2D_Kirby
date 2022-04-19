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
    friend class CPlayer2D;
    friend class CKirbyAttackObjectPool;
public:
    CKirbyAttackEffect();
    CKirbyAttackEffect(const CKirbyAttackEffect& Attack);
    virtual ~CKirbyAttackEffect() override;
protected:
    bool m_BelongToObjectPool;
    bool m_DestroyWhenCollide;
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
    float m_AttackObjectSpeed;
    KirbyAttackEffect_Type m_AttackType;
    class CKirbyState* m_KirbyOwner;
    float m_AttackDamage;
    CSharedPtr<class CCameraComponent> m_Camera;
    class CKirbyAttackObjectPool* m_PoolOwner;
private :
    bool m_CollideShakeEffect;
protected:
    KirbyAttackEffect_Type GetAttackType() const
    {
        return m_AttackType;
    }
    CCameraComponent* GetAttackCamera() const
    {
        return m_Camera;
    }
    CSpriteComponent* GetMainSprite() const
    {
        return m_MainSprite;
    }
protected:
    void SetAttackObjectSpeed(float Speed)
    {
        m_AttackObjectSpeed = Speed;
    }
    void SetAttackObjectMaxLimit(float Speed)
    {
        m_AttackDistLimitMax = Speed;
    }
    void SetDestroyWhenCollide(bool Enable)
    {
        m_DestroyWhenCollide = Enable;
    }
    void SetEnableCollideShakeEffect(bool Enable)
    {
        m_CollideShakeEffect = Enable;
    }
    void SetKirbyAttackEffectMoveSpeed(float Speed)
    {
        m_AttackObjectSpeed = Speed;
    }
    void SetAttackDamage(float AttackDamage)
    {
        m_AttackDamage = AttackDamage;
    }
    void SetKirbyOwner(class CKirbyState* Owner)
    {
        m_KirbyOwner = Owner;
    }
    void SetPoolOwner(class CKirbyAttackObjectPool* Pool)
    {
        m_PoolOwner = Pool;
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
    void MakeBombExplodeEffect();
    void CreateAttackBackEffect(CGameObject* Object);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

