#pragma once
#include "Component\SpriteComponent.h"

// State ���� �� Interface �� �ش��ϴ� �߻� Ŭ���� 
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
    virtual void GoUpAttack() = 0;
protected :
   class  CPlayer2D* m_Player;
protected :
    void SetPlayer(class CPlayer2D* Player)
{
        m_Player = Player;
}
protected:
    virtual void              Start() override;
    virtual bool              Init() override;
    virtual void              Update(float DeltaTime) override;
    virtual void              PostUpdate(float DeltaTime) override;
    virtual void              PrevRender() override;
    virtual void              Render() override;
    virtual void              PostRender() override;
};

