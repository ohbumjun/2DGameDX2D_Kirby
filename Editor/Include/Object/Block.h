#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

class CBlock :
    public CGameObject
{
    friend class CScene;
protected:
    CBlock();
    CBlock(const CBlock& Monster);
    virtual ~CBlock();
protected:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
public:
    void MakeTileTypeAround(bool MakeBlock);
};

