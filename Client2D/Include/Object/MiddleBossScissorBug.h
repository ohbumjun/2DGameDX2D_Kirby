#pragma once
#include "BossMonster.h"

class CMiddleBossScissorBug :
    public CBossMonster
{
    friend class CScene;
protected:
    CMiddleBossScissorBug();
    virtual ~CMiddleBossScissorBug();
private:
    // Reset Init Info
    float m_InitMoveVelocity;
    float m_InitJumpAccel;
    Vector3 m_InitTraceDir;
    Vector3 m_GrabTraceDir;
    // Grab
    bool m_AttemptGrab;
    // Time
    float m_GrabLimitTime;
    float m_GrabLimitTimeMax;
    // Scene Change
    float m_SceneChangeLimitTime;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    // Attack
    void CloseAttack();
    void ChangeCloseAttackAnimation();
    // Grab
    void ChangePrepareGrabAnimation();
    void GrabActionStart();
    void ChangeAttemptGrabAnimation();
    void AttemptGrabEndCallback();
    void UpdateGrabAction(float DeltaTime);
    void ChangeAnimationAfterGrab();
    // Jump & Fly
    void ChangeJumpAttackAnimation();
    void ChangeFlyAnimation();
    void UpdateJumpAction(float DeltaTime);
    // Physics
    void EnablePhysics();
    // Scene Change
    void UpdateSceneChangeLimitTime(float DeltaTime);
    void ChangeSceneToFloat5Scene();
protected:
    virtual void SetObjectLand() override;
    virtual void ChangeTraceAnimation() override;
private:
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
    virtual void AIDeathSpecific(float DeltaTime) override;

};

