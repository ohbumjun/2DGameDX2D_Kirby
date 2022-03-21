#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"

class CWaterFloat1 :
    public CGameObject
{
    friend class CScene;
protected:
    CWaterFloat1();
    CWaterFloat1(const CWaterFloat1& Monster);
    virtual ~CWaterFloat1();
protected:
    CSharedPtr<class CSpriteComponent> m_Sprite;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

