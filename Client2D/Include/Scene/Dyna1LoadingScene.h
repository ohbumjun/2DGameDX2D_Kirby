#pragma once
#include "Scene\SceneMode.h"
#include "../UI/LoadingWidget.h"

class CDyna1LoadingScene :
    public CSceneMode
{
public:
    CDyna1LoadingScene();
    ~CDyna1LoadingScene();
private:
    CSharedPtr<CLoadingWidget> m_LoadingWidget;
    class CLoadingThread* m_LoadingThread;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

