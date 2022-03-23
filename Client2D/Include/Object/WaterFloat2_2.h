#pragma once
#include "GameObject\GameObject.h"
#include "Component/SpriteComponent.h"

class CWaterFloat2_2 :
    public CGameObject
{
    friend class CScene;
protected:
    CWaterFloat2_2();
    virtual ~CWaterFloat2_2();
protected:
    CSharedPtr<class CSpriteComponent> m_Sprite;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

