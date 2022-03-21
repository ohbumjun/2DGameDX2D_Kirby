#pragma once
#include "Monster.h"

class CBossMonster :
    public CMonster
{
    friend class CScene;
protected:
    CBossMonster();
    CBossMonster(const CBossMonster& Monster);
    virtual ~CBossMonster();
protected:
    bool m_StartBossStage;
    Vector2 m_InitWorldResolution;
    // Attack
    float m_AttackResetTime;
    float m_AttackResetTimeMax;
    float m_CloseAttackDistance;
    float m_FarAttackDistance;
protected :
    // Attack Start
    bool m_IsRoundStarted;
protected:
    float m_FarAttackLimitTime;
    float m_FarAttackLimitTimeMax;
protected:
    float m_CloseAttackLimitTime;
    float m_CloseAttackLimitTimeMax;
protected : // Boss 에 따라서 Camera Follow Time을 세팅할 수 있게 한다.
    float m_CameraFollowMaxTime;
protected:
    void MakeBossStartEffect();
    void UpdateAttackResetTime(float DeltaTime);
    void UpdateAttackLimitTimes(float DeltaTime);
protected:
    virtual void AIDeathSpecific(float DeltaTime) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossMonster* Clone() override;
};

