#pragma once
#include "GameObject\GameObject.h"

class CAwl :
    public CGameObject
{
    friend class CScene;
protected:
    CAwl();
    CAwl(const CAwl& Monster);
    virtual ~CAwl();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    void SetCollisionCallback(const CollisionResult& Result);
};

