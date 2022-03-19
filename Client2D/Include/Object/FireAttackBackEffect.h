#pragma once
#include "AttackEffect.h"

class CFireAttackBackEffect :
    public CAttackEffect
{
    friend class CScene;
protected:
    CFireAttackBackEffect();
    CFireAttackBackEffect(const CFireAttackBackEffect& Dash);
    virtual ~CFireAttackBackEffect();
public :
    // void SetRightAttackDir();
    // void SetLeftAttackDir();
    virtual void SetRightAttackDir(float YDir = 0.f) override;
    virtual void SetLeftAttackDir(float YDir = 0.f) override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CFireAttackBackEffect* Clone() override;
};

