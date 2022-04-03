#pragma once
#include "NormalMonster.h"
class CScary :
    public CNormalMonster
{
    friend class CScene;
protected:
    CScary();
    virtual ~CScary();
private:
    bool m_IsToggleGoUp;
    float m_ToggleLimitTime;
    float m_ToggleLimitTimeMax;
private:
    virtual void AIWalkSpecific(float DeltaTime) override;
    virtual void AITraceSpecific(float DeltaTime) override;
private:
    void UpdateToggle(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

