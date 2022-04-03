#pragma once
#include "BossMonster.h"

class CBossDyna :
    public CBossMonster
{
    friend class CScene;
protected:
    CBossDyna();
    virtual ~CBossDyna();
private :
    CSharedPtr<class CSpriteComponent> m_DynaHead;
    CSharedPtr<class CSpriteComponent> m_DynaRightFoot;
    CSharedPtr<class CSpriteComponent> m_DynaLeftFoot;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

