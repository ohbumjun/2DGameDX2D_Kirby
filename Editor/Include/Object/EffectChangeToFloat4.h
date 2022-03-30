#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToFloat4 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToFloat4();
    virtual ~CEffectChangeToFloat4();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Load(FILE* pFile) override;
};

