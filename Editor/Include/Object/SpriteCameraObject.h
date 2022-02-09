#pragma once
#include "GameObject\GameObject.h"

class CSpriteCameraObject :
    public CGameObject
{
    friend class CEditorManager;
    friend class CScene;
private :
    CSpriteCameraObject();
    virtual ~CSpriteCameraObject();
private :
    CSharedPtr<class CCameraComponent> m_Camera;
public :
    CCameraComponent* GetCameraComponent() const
    {
        return m_Camera;
    }
public :
    virtual bool Init();
};

