#pragma once
#include"WaterMonster.h"

class CSquid :
    public CWaterMonster
{
    friend class CScene;
protected:
    CSquid();
    CSquid(const CSquid& Monster);
    virtual ~CSquid();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CSquid* Clone() override;
private :
    virtual void ChangeIdleAnimation() override;
private:
    virtual void Attack() override;
};

