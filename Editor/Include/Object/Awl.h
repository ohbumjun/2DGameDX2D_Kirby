#pragma once
#include "GameObject\GameObject.h"

class CAwl :
    public CGameObject
{
    friend class CScene;
protected:
    CAwl();
    virtual ~CAwl();
protected:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

