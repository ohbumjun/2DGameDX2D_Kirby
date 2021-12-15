#pragma once

#include "GameObject/GameObject.h"

class CBullet :
    public CGameObject
{
    friend class CScene;

protected:
    CBullet();
    CBullet(const CBullet& obj);
    virtual ~CBullet();

private:
    CSharedPtr<class CSpriteComponent>    m_Sprite;
    float   m_Distance;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBullet* Clone();
};

