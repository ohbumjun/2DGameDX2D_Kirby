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
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
public :
    void SetRightAttackDir();
    void SetLeftAttackDir();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CFireAttackBackEffect* Clone() override;
};
