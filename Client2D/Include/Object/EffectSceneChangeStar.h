#pragma once
#include "GameObject\GameObject.h"

class CEffectSceneChangeStar :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectSceneChangeStar();
    CEffectSceneChangeStar(const CEffectSceneChangeStar& Dash);
    virtual ~CEffectSceneChangeStar();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
    CSharedPtr<class CColliderCircle> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectSceneChangeStar* Clone() override;
};

