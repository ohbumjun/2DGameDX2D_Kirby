#pragma once
#include "Monster.h"

class CNormalMonster :
    public CMonster
{
    friend class CScene;
protected :
    CNormalMonster();
    CNormalMonster(const CNormalMonster& Monster);
    virtual ~CNormalMonster();
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime)override;
    virtual void PostUpdate(float DeltaTime)override;
    virtual CNormalMonster* Clone() override;
    virtual void Save(FILE* pFile)override;
    virtual void Load(FILE* pFile)override;
private :
    void OnNormalMonsterBodyCollisionBegin(const CollisionResult& Result);
};

