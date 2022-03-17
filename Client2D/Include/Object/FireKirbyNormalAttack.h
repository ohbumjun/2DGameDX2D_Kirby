#pragma once
#include "GameObject\GameObject.h"

class CFireKirbyNormalAttack :
    public CGameObject
{
    friend class CFireKirbyState;
public:
    CFireKirbyNormalAttack();
    CFireKirbyNormalAttack(const CFireKirbyNormalAttack& Attack);
    virtual ~CFireKirbyNormalAttack() override;
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    float m_AttackDir;
    float m_AttackDistLimit;
    float m_AttackDistLimitMax;
protected:
    void SetRightAttackDir();
    void SetLeftAttackDir();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

