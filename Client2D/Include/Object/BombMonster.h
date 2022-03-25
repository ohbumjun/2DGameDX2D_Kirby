#pragma once
#include "AbilityMonster.h"

class CBombMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CBombMonster();
    CBombMonster(const CBombMonster& Monster);
    virtual ~CBombMonster();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBombMonster* Clone() override;
private:
    virtual void Attack() override;
};

