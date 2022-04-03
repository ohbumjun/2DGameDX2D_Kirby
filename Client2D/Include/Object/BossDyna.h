#pragma once
#include "BossMonster.h"

class CBossDyna :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossDyna();
    virtual ~CBossDyna() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    float m_SceneChangeLimitTime;
    CSharedPtr<class CSpriteComponent> m_DynaHead;
    CSharedPtr<class CSpriteComponent> m_DynaRightFoot;
    CSharedPtr<class CSpriteComponent> m_DynaLeftFoot;
private :
    float m_HeadToggleMaxTime;
    float m_HeadToggleTime;
    bool m_HeadToggleUp;
    float m_InitHeadYRelativePos;
private:
    // Movement
    bool m_IsAppearing;
    bool m_MovementRight;
    bool m_MovementUp;
    float m_MovementTargetYPos;
private:
    void FarAttack();
    void CloseAttack();
    void ChangeFarAttackAnimation();
    void ChangeCloseAttackAnimation();
    void UpdateSceneChangeLimitTime(float DeltaTime);
    void ChangeSceneToFloat1Scene();
protected:
    virtual void ChangeTraceAnimation() override;
    void UpdateScale(float DeltaTime);
    void UpdateHeadToggle(float DeltaTime);
    void UpdateMovement(float DeltaTime);
    void UpdateAppearance(float DeltaTime);
private:
    virtual void AIAttackSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
    virtual void AIWalkSpecific(float DeltaTime) override;
    virtual void AIIdleSpecific(float DeltaTime) override;
    virtual void AIHitSpecific(float DeltaTime) override;
    virtual void AIDeathSpecific(float DeltaTime) override;
};

