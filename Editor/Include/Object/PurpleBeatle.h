#pragma once
#include "Object/NormalMonster.h"

class CPurpleBeatle :
    public CNormalMonster
{
    friend class CScene;
protected:
    CPurpleBeatle();
    virtual ~CPurpleBeatle()override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
    virtual void PostUpdate(float DeltaTime)override;
    virtual CPurpleBeatle* Clone() override;
};

