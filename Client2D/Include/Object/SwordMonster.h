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
private:
    virtual void Attack() override;
    virtual void AITraceSpecific(float DeltaTime) override;
};

