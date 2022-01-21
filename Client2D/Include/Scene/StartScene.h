#pragma once

#include "Scene\SceneMode.h"
#include "../../UI/StartWidget.h"

class CStartScene :
    public CSceneMode
{
public :
    CStartScene();
    ~CStartScene();
private :
    CSharedPtr<CStartWidget> m_StartWidget;
public :
    virtual bool Init() override;
};

