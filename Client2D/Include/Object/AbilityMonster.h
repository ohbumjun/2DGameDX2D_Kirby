#pragma once

#include "Monster.h"

class CAbilityMonster :
    public CMonster
{
    friend class CScene;
protected:
    CAbilityMonster();
    CAbilityMonster(const CAbilityMonster& Monster); //
    virtual ~CAbilityMonster();
protected :
    Ability_State m_AbilityState;
    float m_AttackResetTime;
    float m_AttackResetTimeMax;
    float m_AttackLimitTime;
    float m_AttackLimitTimeMax;
    
public:
    Ability_State GetAbilityState() const
    {
        return m_AbilityState;
    }
public :
    void SetAbilityState(Ability_State State)
    {
        m_AbilityState = State;
    }
protected :
    void UpdateAttackLimitTime(float DeltaTime);
    virtual void Attack();
    virtual void ChangeAttackAnimation() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAbilityMonster* Clone() override;
};

