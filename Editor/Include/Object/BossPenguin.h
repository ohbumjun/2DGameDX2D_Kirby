#pragma once
#include "BossMonster.h"

class CBossPenguin :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossPenguin();
    CBossPenguin(const CBossPenguin& Monster);
    virtual ~CBossPenguin();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBossPenguin* Clone() override;
};

