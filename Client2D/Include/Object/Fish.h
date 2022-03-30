#pragma once
#include "WaterMonster.h"
class CFish :
    public CWaterMonster
{
    friend class CScene;
protected:
    CFish();
    virtual ~CFish();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    virtual void ChangeIdleAnimation() override;
private:
    virtual void Attack() override;
};

