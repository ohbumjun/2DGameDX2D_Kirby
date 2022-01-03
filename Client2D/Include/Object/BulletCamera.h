#pragma once
#include "GameObject\GameObject.h"


class CBulletCamera :
    public CGameObject
{
    friend class CScene;
protected :
    CBulletCamera();
    CBulletCamera(const CBulletCamera& Camera);
    virtual ~CBulletCamera() override;
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderBox2D> m_Body;
    CSharedPtr<class CCameraComponent> m_Camera;
    float m_Distance;
public :
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBulletCamera* Clone();
private :
    void CollisionCallback(const CollisionResult& result);
};

