#pragma once
#include "GameObject\GameObject.h"
#include "../Component/FightKirbyState.h"
#include "../Component/KirbyState.h"
#include "../Component/BeamKirbyState.h"
#include "../Component/FireKirbyState.h"
#include "../Component/BombKirbyState.h"
#include "../Component/SwordKirbyState.h"
#include "../Component/NormalKirbyState.h"
#include "../Client.h"
#include "Animation/AnimationSequence2DData.h"
#include "Animation/AnimationSequence2DInstance.h"

class CPlayerClone :
    public CGameObject
{
    friend class CPlayer2D;
    friend class CScene;
public:
    CPlayerClone();
    virtual ~CPlayerClone();
private :
    CSharedPtr<CKirbyState> m_KirbyState;
    float m_ObjectMoveDir;
    float m_ObjectMoveSpeed;
    float m_OpacityDecreaseMaxTime;
    float m_OpacityDecreaseTime;
private :
    void SetKirbyState(Ability_State State);
private :
    CKirbyState* GetKirbyState() const
    {
        return m_KirbyState;
    }
    CAnimationSequence2DInstance* GetAnimationInstance() const
{
        return m_KirbyState->GetAnimationInstance();
}
private :
    void SetObjectMoveDir(float Dir)
{
        m_ObjectMoveDir = Dir;
}
    void SetObjectMoveSpeed(float Speed)
{
        m_ObjectMoveSpeed = Speed;
}
    void SetOpacityDecreaseTime(float Time)
{
        m_OpacityDecreaseMaxTime = Time;
}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
};

