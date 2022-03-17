#pragma once
#include "GameObject\GameObject.h"

class CFightKirbyNormalAttack :
    public CGameObject
{
    friend class CFireMonster;
public:
    CFightKirbyNormalAttack();
    CFightKirbyNormalAttack(const CFightKirbyNormalAttack& Attack);
    virtual ~CFightKirbyNormalAttack() override;
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

