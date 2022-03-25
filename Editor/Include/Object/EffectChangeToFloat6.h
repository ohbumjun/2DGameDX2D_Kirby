#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToFloat6 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToFloat6();
    virtual ~CEffectChangeToFloat6();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};
