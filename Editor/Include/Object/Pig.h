#pragma once
#include "NormalMonster.h"
class CPig :
    public CNormalMonster
{
    friend class CScene;
protected:
    CPig();
    virtual ~CPig();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

