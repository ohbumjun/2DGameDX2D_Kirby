#pragma once
#include "NormalMonster.h"

class CBossDynaBaby :
    public CNormalMonster
{
    friend class CScene;
    friend class CDynaNest;
    friend class CBossDyna;
protected:
    CBossDynaBaby();
    virtual ~CBossDynaBaby();
private:
    bool m_IsToggleGoUp;
    float m_ToggleLimitTime;
    float m_ToggleLimitTimeMax;
private :
    class CBossDyna* m_BossDyna;
private:
    virtual void AIWalkSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
    virtual void AIDeathSpecific(float DeltaTime) override;
private:
    void UpdateToggle(float DeltaTime);
    void JumpStart();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

