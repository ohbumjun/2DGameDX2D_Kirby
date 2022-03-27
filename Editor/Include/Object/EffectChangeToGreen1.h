#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToGreen1 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToGreen1();
    virtual ~CEffectChangeToGreen1();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

