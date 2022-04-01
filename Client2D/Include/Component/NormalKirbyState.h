#pragma once
#include "KirbyState.h"

class CNormalKirbyState :
    public CKirbyState
{
    friend class CPlayer2D;
    friend class CGameObject;
public :
    CNormalKirbyState();
    CNormalKirbyState(const CNormalKirbyState& Kirby);
    virtual ~CNormalKirbyState();
protected:
    virtual void Attack();
    virtual void FallDownAttack();
    virtual void GoUpAttack();
    virtual void SpecialAttack(){}
    virtual void UltimateAttack() {}
protected:
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
    virtual CNormalKirbyState* Clone() override;
};

