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
protected:
    void MakeBossStartEffect();
protected:
    virtual void AIDeathSpecific(float DeltaTime) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossMonster* Clone() override;
};

