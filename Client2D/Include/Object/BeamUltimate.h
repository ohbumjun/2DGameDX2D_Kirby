#pragma once
#include "KirbyAttackEffect.h"

class CBeamUltimate :
    public CKirbyAttackEffect
{
    friend class CScene;
    friend class CBeamKirbyState;
private :
    CBeamUltimate();
    virtual ~CBeamUltimate() override;
private :
    float m_InitStopTime;
    float m_EnableToggleTime;
    float m_EnableCurTime;
    bool m_ShowEnable;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
};


