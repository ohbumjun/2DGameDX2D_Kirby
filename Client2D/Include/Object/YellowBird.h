#pragma once
#include "NormalMonster.h"

class CYellowBird :
    public CNormalMonster
{
    friend class CScene;
protected:
    CYellowBird();
    CYellowBird(const CYellowBird& Bird);
    virtual ~CYellowBird();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CYellowBird* Clone() override;
};

