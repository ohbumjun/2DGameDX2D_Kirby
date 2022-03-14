#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToGreen2 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToGreen2();
    CEffectChangeToGreen2(const CEffectChangeToGreen2& Dash);
    virtual ~CEffectChangeToGreen2();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectChangeToGreen2* Clone() override;
};

