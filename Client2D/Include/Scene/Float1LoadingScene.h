#pragma once
#include "Scene\SceneMode.h"
#include "../UI/LoadingWidget.h"

class CFloat1LoadingScene :
    public CSceneMode
{
public:
    CFloat1LoadingScene();
    ~CFloat1LoadingScene();
private :
    CSharedPtr<CLoadingWidget> m_LoadingWidget;
    class CLoadingThread* m_LoadingThread;

public :
    virtual bool Init();
    virtual void Update(float DeltaTime);
};

