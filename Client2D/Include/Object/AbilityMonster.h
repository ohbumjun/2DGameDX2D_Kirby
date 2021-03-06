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
    virtual void Attack();
    virtual void ChangeAttackAnimation() override;
    virtual void AIDeathSpecific(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAbilityMonster* Clone() override;
};

