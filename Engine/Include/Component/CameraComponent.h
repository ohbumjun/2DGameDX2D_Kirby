#pragma once
#include "SceneComponent.h"
class CCameraComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCameraManager;
protected :
    CCameraComponent();
    CCameraComponent(const CCameraComponent& com);
    virtual ~CCameraComponent();
protected :
    Camera_Type m_CameraType;
    Resolution m_RS;
    float m_ViewAngle;
    float m_Distance;
    Matrix m_matProj;
    Matrix m_matView;
public :
    Camera_Type GetCameraType() const
{
        return m_CameraType;
}
    Resolution GetResolution() const
{
        return m_RS;
}
};

