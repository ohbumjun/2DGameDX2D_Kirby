#pragma once
#include "GameObject\GameObject.h"
#include "Component/CameraComponent.h"

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
    CSharedPtr<CCameraComponent>     m_Camera;
    Resolution m_RS;
private :
    float m_YToggleDir;
public :
    CCameraComponent* GetCameraComponent() const
{
        return m_Camera;
}
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectKirbyRide* Clone() override;
};

