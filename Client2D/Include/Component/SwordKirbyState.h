#pragma once
#include "KirbyState.h"
class CSwordKirbyState :
    public CKirbyState
{
    friend class CPlayer2D;
    friend class CGameObject;
public:
    CSwordKirbyState();
    virtual ~CSwordKirbyState();
protected:
    virtual void Attack();
    virtual void FallDownAttack();
    virtual void GoUpAttack();
    virtual void SpecialAttack();
private:
    // Go Up
    bool m_GoUpState;
    bool m_GoUpReachedTop;
    float m_GoUpDist;
    Vector3 m_InitWorldScale;
    // Fall Attack
    bool m_FallAttackState;
    float m_FallAttackTime;
    float m_FallAttackTimeMax;
    // Init Collider Scale
    Vector2 m_InitColliderLength;
private:
    void UpdateAttackGoUpState(float DeltaTime);
    void UpdateFallAttack(float DeltaTime);
public:
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
private:
    void NormalAttackCallback();
};

