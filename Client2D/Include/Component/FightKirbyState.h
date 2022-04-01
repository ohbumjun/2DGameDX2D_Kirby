#pragma once
#include "KirbyState.h"

class CFightKirbyState :
    public CKirbyState
{
    friend class CPlayer2D;
    friend class CGameObject;
public :
    CFightKirbyState();
    CFightKirbyState(const CFightKirbyState& Kirby);
    virtual ~CFightKirbyState();
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
    float m_InitAttackAbility;
    bool m_InitScaleSet;
    Vector3 m_InitWorldScale;

	// Fall Attack
    bool m_FallAttackState;
    float m_FallAttackTime;
    float m_FallAttackTimeMax;
    Vector2 m_InitColliderLength;

    // Special Attack
    bool m_IsSpecialAttack;
    float m_SpecialAttackTime;
    float m_SpecialAttackTimeMax;
    float m_SpecialAttackDir;
private :
    void UpdateAttackGoUpState(float DeltaTime);
    void UpdateFallAttack(float DeltaTime);
    void UpdateSpecialAttack(float DeltaTime);
public :
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
    virtual CFightKirbyState* Clone() override;
private:
    void NormalAttackCallback();
};

