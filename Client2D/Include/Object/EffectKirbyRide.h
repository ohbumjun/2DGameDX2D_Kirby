#pragma once
#include "GameObject\GameObject.h"

class CEffectKirbyRide :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectKirbyRide();
    CEffectKirbyRide(const CEffectKirbyRide& Dash);
    virtual ~CEffectKirbyRide();
private:
    CSharedPtr<class CSpriteComponent> m_Sprite;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectKirbyRide* Clone() override;
};

