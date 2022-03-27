#pragma once
#include "KirbyState.h"

class CBombKirbyState :
    public CKirbyState
{
    friend class CPlayer2D;
    friend class CGameObject;
public:
    CBombKirbyState();
    virtual ~CBombKirbyState();
protected:
    virtual void Attack();
    virtual void FallDownAttack();
    virtual void GoUpAttack();
    virtual void SpecialAttack();
protected :
    void MakeFallDownBomb();
private:
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
    bool m_FallDownBombMade;
    // Attack
    bool m_AttackReady;
    CSharedPtr<class CBombKirbyThrowBomb> m_ThrowReadyBomb;
public :
    void SetFallAttackEnd();
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

