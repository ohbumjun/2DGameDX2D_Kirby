#pragma once
#include "BossMonster.h"

class CMiddleBossScissorBug :
    public CBossMonster
{
    friend class CScene;
protected:
    CMiddleBossScissorBug();
    virtual ~CMiddleBossScissorBug();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

