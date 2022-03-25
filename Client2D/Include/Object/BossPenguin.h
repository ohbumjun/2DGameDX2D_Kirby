#pragma once
#include "BossMonster.h"

class CBossPenguin :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossPenguin();
    virtual ~CBossPenguin();
private:
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
    // Far Attack
    bool m_IsFarAttacking;
    float m_FarAttackLimitTime;
    float m_FarAttackLimitTimeMax;
    Vector3 m_FarAttackTraceDir;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    void FarAttack();
    void CloseAttack();
    void ChangeFarAttackAnimation();
    void ChangeCloseAttackAnimation();
    void ChangeJumpAttackAnimation();
    void UpdateJumpAction(float DeltaTime);
    void UpdateFarAttackAction(float DeltaTime);
protected:
    virtual void SetObjectLand() override;
    virtual void ChangeTraceAnimation() override;
private:
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
};

