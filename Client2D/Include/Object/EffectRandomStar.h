#pragma once
#include "GameObject\GameObject.h"

class CEffectRandomStar :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectRandomStar();
    CEffectRandomStar(const CEffectRandomStar& Dash);
    virtual ~CEffectRandomStar();
private:
    Vector3 m_RandomDir;
    CSharedPtr<class CSpriteComponent> m_Sprite;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectRandomStar* Clone() override;
};

