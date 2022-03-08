#pragma once
#include "Component\SpriteComponent.h"

class CKirbyState :
    public CSpriteComponent
{
    friend class CPlayer2D;
    friend class CGameObject;
protected:
    CKirbyState();
    CKirbyState(const CKirbyState& Kirby);
    virtual ~CKirbyState();
protected:
    virtual void Attack() = 0;
    virtual void FallDownAttack() = 0;
protected:
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
};

