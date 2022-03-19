#pragma once
#include "AbilityMonster.h"

class CFightMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CFightMonster();
    CFightMonster(const CFightMonster& Monster);
    virtual ~CFightMonster();
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
    virtual CFightMonster* Clone() override;
private:
    void Attack();
};
