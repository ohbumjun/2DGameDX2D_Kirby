#pragma once
#include "NormalMonster.h"

class CChicken :
    public CNormalMonster
{
    friend class CScene;
protected:
    CChicken();
    virtual ~CChicken();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

