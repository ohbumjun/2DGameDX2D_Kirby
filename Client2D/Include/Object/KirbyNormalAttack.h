#pragma once
#include "AttackEffect.h"
#include "../Client.h"

class CKirbyNormalAttack :
    public CAttackEffect
{
    friend class CFireKirbyState;
    friend class CFightKirbyState;
    friend class CBeamKirbyState;
public:
    CKirbyNormalAttack();
    CKirbyNormalAttack(const CKirbyNormalAttack& Attack);
    virtual ~CKirbyNormalAttack() override;
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    Vector2 m_AttackDir;
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
    float m_AttackObjectSpeed;
    KirbyNormalAttack_Type m_AttackType;
protected:
    void SetRightAttackDir(float YDir);
    void SetLeftAttackDir(float YDir);
    void SetAttackType(KirbyNormalAttack_Type Type);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

