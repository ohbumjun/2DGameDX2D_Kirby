#pragma once
#include "NormalMonster.h"

class CKabu :
    public CNormalMonster
{
    friend class CScene;
protected:
    CKabu();
    virtual ~CKabu()override;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};

