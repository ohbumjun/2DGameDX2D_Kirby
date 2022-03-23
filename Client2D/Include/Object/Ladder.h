#pragma once
#include "GameObject\GameObject.h"

class CLadder :
    public CGameObject
{
    friend class CScene;
protected:
    CLadder();
    CLadder(const CLadder& Monster);
    virtual ~CLadder();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
private:
    void SetBeginCollisionCallback(const CollisionResult& Result);
    void SetEndCollisionCallback(const CollisionResult& Result);
};

