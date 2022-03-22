#pragma once
#include "AttackEffect.h"

class CApple :
    public CAttackEffect
{
    friend class CMiddleBossHammer;
public:
    CApple();
    CApple(const CApple& Attack);
    virtual ~CApple() override;
protected:
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
private :
    virtual void BottomCollisionSpecificAction() override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

