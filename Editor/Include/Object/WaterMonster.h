#pragma once
#include "Monster.h"

class CWaterMonster :
    public CMonster
{
    friend class CScene;
protected:
    CWaterMonster();
    virtual ~CWaterMonster() = 0;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
    virtual void PostUpdate(float DeltaTime)override;
};

