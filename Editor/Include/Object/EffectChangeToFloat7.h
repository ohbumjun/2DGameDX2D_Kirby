#pragma once
#include "GameObject\GameObject.h"

class CEffectChangeToFloat7 :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectChangeToFloat7();
    virtual ~CEffectChangeToFloat7();
private:
    CSharedPtr<class CColliderBox2D> m_ColliderBody;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
};

