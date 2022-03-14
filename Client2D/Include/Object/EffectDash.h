#pragma once
#include "GameObject\GameObject.h"

class CEffectDash :
    public CGameObject
{
    friend class CScene;
protected:
    CEffectDash();
    CEffectDash(const CEffectDash& Dash);
    virtual ~CEffectDash();
private :
    bool m_GoRight;
    CSharedPtr<class CSpriteComponent> m_Sprite;
public :
    void SetDirGoRight(bool Enable);
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CEffectDash* Clone() override;
};

