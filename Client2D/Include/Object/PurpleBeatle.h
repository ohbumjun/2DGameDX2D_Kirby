#pragma once
#include "NormalMonster.h"

class CPurpleBeatle :
    public CNormalMonster
{
    friend class CScene;
protected:
    CPurpleBeatle();
    CPurpleBeatle(const CPurpleBeatle& Beatle);
    virtual ~CPurpleBeatle();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPurpleBeatle* Clone() override;
};

