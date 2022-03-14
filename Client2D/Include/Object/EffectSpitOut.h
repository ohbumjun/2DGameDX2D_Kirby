#pragma once
#include "GameObject\GameObject.h"

class CEffectSpitOut :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectSpitOut();
    CEffectSpitOut(const CEffectSpitOut& Dash);
    virtual ~CEffectSpitOut();
private:
    bool m_GoRight;
    CSharedPtr<class CSpriteComponent> m_Sprite;
public:
    void SetDirGoRight(bool Enable);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectSpitOut* Clone() override;
};

