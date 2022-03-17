#pragma once
#include "C:\Users\dhsys\Desktop\DX_Project\Engine\Include\GameObject\GameObject.h"

class CFireKirbyNormalAttack :
    public CGameObject
{
    friend class CFireMonster;
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
    void SetRightAttackDir(float YDir);
    void SetLeftAttackDir(float YDir);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
private:
    void CollisionCallback(const CollisionResult& Result);
};

