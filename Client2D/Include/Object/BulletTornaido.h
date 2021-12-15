#pragma once

#include "GameObject/GameObject.h"

class CBulletTornaido :
    public CGameObject
{
    friend class CScene;

protected:
    CBulletTornaido();
    CBulletTornaido(const CBulletTornaido& obj);
    virtual ~CBulletTornaido();

private:
    CSharedPtr<class CSceneComponent>    m_Root;
    CSharedPtr<class CSceneComponent>    m_Root1;
    CSharedPtr<class CSpriteComponent>    m_Sprite;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBulletTornaido* Clone();
};

