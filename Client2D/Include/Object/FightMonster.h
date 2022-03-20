#pragma once
#include "AbilityMonster.h"

class CFightMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CFightMonster();
    CFightMonster(const CFightMonster& Monster);
    virtual ~CFightMonster();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CFightMonster* Clone() override;
private:
    virtual void Attack() override;
};

