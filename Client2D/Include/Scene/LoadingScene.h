#pragma once
#include "Scene\SceneMode.h"
#include "../../UI/LoadingWidget.h"

class CLoadingScene :
    public CSceneMode
{
public :
    CLoadingScene();
    ~CLoadingScene();
private :
    CSharedPtr<CLoadingWidget> m_LoadingWidget;
public :
    virtual bool Init() override;
};

