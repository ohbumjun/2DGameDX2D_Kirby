#pragma once
#include "Object\BossMonster.h"

class CMiddleBossHammer :
    public CBossMonster
{
    friend class CScene;
protected:
    CMiddleBossHammer();
    virtual ~CMiddleBossHammer();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

