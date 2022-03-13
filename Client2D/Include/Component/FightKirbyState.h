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
public :
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
    virtual CFightKirbyState* Clone() override;
};
