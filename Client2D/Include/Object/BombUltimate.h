#pragma once
#include "KirbyAttackEffect.h"

class CBombUltimate :
    public CKirbyAttackEffect
{
    friend class CScene;
    friend class CBombKirbyState;
private:
    CBombUltimate();
    virtual ~CBombUltimate() override;
private:
    float m_ExplodeWaitTime;
    float m_ExplodeCurTime;
    bool m_CameraReturned;
public:
    virtual void Start() override;
    virtual bool Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void PostUpdate(float DeltaTime) override;
private :
    void CreateMultipleExplosionEffect();
};

