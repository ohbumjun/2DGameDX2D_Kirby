#pragma once
#include "NormalMonster.h"

class CNormalBear :
    public CNormalMonster
{
    friend class CScene;
protected:
    CNormalBear();
    CNormalBear(const CNormalBear& Beatle);
    virtual ~CNormalBear();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CNormalBear* Clone() override;
};

