#pragma once
#include "KirbyState.h"

class CFireKirbyState :
    public CKirbyState
{
    friend class CPlayer2D;
    friend class CGameObject;
public:
    CFireKirbyState();
    CFireKirbyState(const CFireKirbyState& Kirby);
    virtual ~CFireKirbyState();
protected:
    virtual void Attack();
    virtual void FallDownAttack();
    virtual void GoUpAttack();
    virtual void SpecialAttack();
    virtual void UltimateAttack() override;
private :
    // Go Up
    bool m_GoUpState;
    float m_GoUpTime;
    float m_GoUpTimeMax;
    bool m_InitScaleSet;
    // Fall Attack
    bool m_FallAttackState;
    float m_FallAttackTime;
    float m_FallAttackTimeMax;
    // Init Collider Scale
    Vector2 m_InitColliderLength;
    // Special Attack
    int m_NumSpecialAttack;
    bool m_IsSpecialAttack;
    float m_SpecialAttackTime;
private:
    void UpdateAttackGoUpState(float DeltaTime);
    void UpdateFallAttack(float DeltaTime);
    void UpdateSpecialAttack(float DeltaTime);
    void CreateAttackEffect();
public:
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
    virtual CFireKirbyState* Clone() override;
private:
    void NormalAttackCallback();
};

