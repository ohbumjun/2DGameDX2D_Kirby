#pragma once
#include "GameObject\GameObject.h"

class CBossFightParticle :
    public CGameObject
{
    friend class CScene;
protected:
    CBossFightParticle();
    virtual ~CBossFightParticle() override;
private:
    CSharedPtr<class CParticleComponent> m_Particle;
public:
    virtual bool Init() override;
};

