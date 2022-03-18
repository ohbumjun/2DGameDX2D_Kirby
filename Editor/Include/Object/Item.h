#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

class CItem :
    public CGameObject
{
    friend class CScene;
protected:
    CItem();
    CItem(const CItem& Monster);
    virtual ~CItem() = 0;
protected :
    Item_Type m_ItemType;
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
public :
    virtual void Save(FILE* pFile) override;
};

