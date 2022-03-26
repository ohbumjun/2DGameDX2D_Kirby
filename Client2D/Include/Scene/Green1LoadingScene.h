#pragma once
#include "Scene\SceneMode.h"
#include "../../UI/LoadingWidget.h"

class CGreen1LoadingScene :
    public CSceneMode
{
public :
    CGreen1LoadingScene();
    ~CGreen1LoadingScene();
private :
    CSharedPtr<CLoadingWidget> m_LoadingWidget;
    class CLoadingThread* m_LoadingThread;
public :
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
};

