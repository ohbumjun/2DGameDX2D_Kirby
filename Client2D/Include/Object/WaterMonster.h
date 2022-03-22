#pragma once
#include "Monster.h"

class CWaterMonster :
    public CMonster
{
    friend class CScene;
protected:
    CWaterMonster();
    CWaterMonster(const CWaterMonster& Monster);
    virtual ~CWaterMonster();
private :
    float m_WaterGoUpTime;
    float m_WaterGoUpTimeMax;
    bool m_GoUp;
protected:
    void UpdateWaterGoUpTime(float DeltaTime);
    void CheckBelowWorldResolution() override;
    void UpdateSwimEffect(float DeltaTime);
    virtual void Attack();
    virtual void ChangeAttackAnimation() override;
    virtual void AIDeathSpecific(float DeltaTime);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CWaterMonster* Clone() override;
};

