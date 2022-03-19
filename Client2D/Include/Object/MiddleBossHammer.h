#pragma once
#include "BossMonster.h"

class CMiddleBossHammer :
    public CBossMonster
{
    friend class CScene;
protected:
    CMiddleBossHammer();
    CMiddleBossHammer(const CMiddleBossHammer& Monster);
    virtual ~CMiddleBossHammer();
private:
    bool m_IsAttacking;
    float m_AttackResetTime;
    float m_AttackResetTimeMax;
public:
    void SetAttackEnd()
    {
        m_IsAttacking = false;
    }
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMiddleBossHammer* Clone() override;
private:
    void Attack();
};

