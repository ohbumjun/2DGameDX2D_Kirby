#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToGreen5 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToGreen5();
    CEffectChangeToGreen5(const CEffectChangeToGreen5& Dash);
    virtual ~CEffectChangeToGreen5();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectChangeToGreen5* Clone() override;
};

