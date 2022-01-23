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
    class CLoadingThread* m_LoadingThread;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};

