#pragma once
#include "GameObject\GameObject.h"
#include "Component/ColliderCircle.h"

class CBlock :
    public CGameObject
{
    friend class CScene;
protected:
    CBlock();
    CBlock(const CBlock& Monster);
    virtual ~CBlock();
private:
    CSharedPtr<class CColliderCircle> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
public:
    void MakeTileTypeAround(bool MakeBlock);
private :
    void SetCollisionCallback(const CollisionResult& Result);
};

