#pragma once
#include "AbilityMonster.h"

class CSwordMonster :
    public CAbilityMonster
{
    friend class CScene;
protected:
    CSwordMonster();
    virtual ~CSwordMonster();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

