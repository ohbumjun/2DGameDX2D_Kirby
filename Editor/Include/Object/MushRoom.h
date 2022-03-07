#pragma once
#include "NormalMonster.h"

class CMushRoom :
    public CNormalMonster
{
    friend class CScene;
protected:
    CMushRoom();
    CMushRoom(const CMushRoom& Beatle);
    virtual ~CMushRoom();
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMushRoom* Clone() override;
};

