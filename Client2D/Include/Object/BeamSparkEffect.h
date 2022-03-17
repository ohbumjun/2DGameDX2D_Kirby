#pragma once
#include "AttackEffect.h"
class CBeamSparkEffect :
    public CAttackEffect
{
protected:
    CBeamSparkEffect();
    CBeamSparkEffect(const CBeamSparkEffect& Dash);
    virtual ~CBeamSparkEffect();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
public:
    void SetRightAttackDir();
    void SetLeftAttackDir();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBeamSparkEffect* Clone() override;
};

