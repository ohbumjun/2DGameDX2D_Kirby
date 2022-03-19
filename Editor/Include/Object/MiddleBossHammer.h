#pragma once
#include "Object\Monster.h"

class CMiddleBossHammer :
    public CMonster
{
    friend class CScene;
protected:
    CMiddleBossHammer();
    CMiddleBossHammer(const CMiddleBossHammer& Monster);
    virtual ~CMiddleBossHammer();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMiddleBossHammer* Clone() override;
};

