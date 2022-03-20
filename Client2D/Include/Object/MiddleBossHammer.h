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
    // Attack
    bool m_IsAttacking;
    float m_AttackResetTime;
    float m_AttackResetTimeMax;
    float m_CloseAttackDistance;
    float m_FarAttackDistance;
    // Jump
    bool m_JumpEnable;
    float m_JumpLimitTime;
    float m_JumpLimitTimeMax;
    float m_JumpDistance;
    float m_DashRunDistance;
    // Reset Init Info
    float m_InitMoveVelocity;
    float m_InitJumpAccel;
    Vector3 m_InitTraceDir;
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
    void UpdateJumpAction(float DeltaTime);
protected :
    virtual void SetObjectLand() override;
    virtual void ChangeTraceAnimation() override;
private :
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
};

