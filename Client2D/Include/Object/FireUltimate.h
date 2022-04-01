#pragma once
#include "KirbyAttackEffect.h"

class CFireUltimate :
    public CKirbyAttackEffect
{
    friend class CScene;
    friend class CFireKirbyState;
private:
    CFireUltimate();
    virtual ~CFireUltimate() override;
private:
    float m_InitHideTime;
    float m_MoveStartTime;
    float m_MoveReadyTime;
    bool m_MoveStart;
private :
    void SetInitHideTime(float Time)
{
        m_InitHideTime = Time;
}
    void SetMoveStartTime(float Time)
{
        m_MoveStartTime = Time;
}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

