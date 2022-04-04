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
protected : // Boss �� ���� Camera Follow Time�� ������ �� �ְ� �Ѵ�.
    float m_CameraFollowMaxTime;
protected :
    bool m_BossAngry;
protected:
    void MakeBossStartEffect();
    void UpdateAttackLimitTimes(float DeltaTime);
    void UpdateBossAngryEffect(float DeltaTime);
    virtual void MakeBossAngry();
    void MakeBossFightParticleEffect();
 protected:
    virtual void AIDeathSpecific(float DeltaTime) override;
    void StartPaperBurnEffect();
    void PaperBurnEndCallback();
    virtual void Damage(float Damage) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossMonster* Clone() override;
};

