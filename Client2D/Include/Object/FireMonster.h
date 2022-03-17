#pragma once
#include "AbilityMonster.h"

class CFireMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CFireMonster();
    CFireMonster(const CFireMonster& Monster);
    virtual ~CFireMonster();
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
    virtual CFireMonster* Clone() override;
private:
    void Attack();
    virtual void AITraceSpecific(float DeltaTime) override;
};

