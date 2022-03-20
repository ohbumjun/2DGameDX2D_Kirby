#pragma once

#include "AbilityMonster.h"

class CBeamMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CBeamMonster();
    CBeamMonster(const CBeamMonster& Monster);
    virtual ~CBeamMonster();
private :
    CSharedPtr<class CBeamMonsterAttack> m_AttackEffect;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBeamMonster* Clone() override;
private :
    void Attack();
};

