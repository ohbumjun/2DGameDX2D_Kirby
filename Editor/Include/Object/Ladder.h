#pragma once
#include "GameObject\GameObject.h"

class CLadder :
    public CGameObject
{
    friend class CScene;
protected:
    CLadder();
    virtual ~CLadder();
protected:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

