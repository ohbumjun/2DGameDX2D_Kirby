#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToGreen4 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToGreen4();
    CEffectChangeToGreen4(const CEffectChangeToGreen4& Dash);
    virtual ~CEffectChangeToGreen4();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
    CSharedPtr<class CWidgetComponent> m_SimpleHUDWidget;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectChangeToGreen4* Clone() override;
};

