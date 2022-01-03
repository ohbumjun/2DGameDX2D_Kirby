#pragma once
#include "GameObject\GameObject.h"
class CBulletCamera :
    public CGameObject
{
    friend class CScene;
protected :
    CBulletCamera();
    CBulletCamera(const CBulletCamera& Bullet);
    virtual ~CBulletCamera();
private :
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderBox2D> m_Body;
    CSharedPtr<class CCameraComponent> m_Camera;
    float m_Distance;
public :
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBulletCamera* Clone();
public:

};

