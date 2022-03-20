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
    float m_CloseAttackDistance;
    float m_FarAttackDistance;
    bool m_JumpEnable;
    float m_JumpLimitTime;
    float m_JumpLimitTimeMax;
    float m_JumpDistance;
    float m_DashRunDistance;
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
    void FarAttack();
    void CloseAttack();
    void ChangeFarAttackAnimation();
    void ChangeCloseAttackAnimation();
    void ChangeJumpAttackAnimation();
    void ChangeJumpEndAttackAnimation();
    void ChangeToIdleAfterHit();
protected :
    virtual void ChangeTraceAnimation() override;
private :
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
};

