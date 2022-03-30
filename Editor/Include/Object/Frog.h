#pragma once
#include "NormalMonster.h"

class CFrog :
    public CNormalMonster
{
    friend class CScene;
protected:
    CFrog();
    virtual ~CFrog();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

