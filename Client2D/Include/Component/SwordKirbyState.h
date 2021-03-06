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
    virtual void UltimateAttack() override;
private:
    // Go Up
    bool m_GoUpState;
    bool m_GoUpReachedTop;
    float m_GoUpDist;
    // Fall Attack
    bool m_FallAttackState;
    float m_FallAttackTime;
    float m_FallAttackTimeMax;
    // Init Collider Scale
    Vector2 m_InitColliderLength;
    float m_SpecialAttackTime;
    bool m_IsSpecialAttack;
private:
    void UpdateAttackGoUpState(float DeltaTime);
    void UpdateFallAttack(float DeltaTime);
    void UpdateSpecialAttack(float DeltaTime);
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

