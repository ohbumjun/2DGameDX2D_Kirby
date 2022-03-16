#pragma once
#include "GameObject\GameObject.h"

class CMoonAboveParticle :
    public CGameObject
{
    friend class CScene;
protected:
    CMoonAboveParticle();
    CMoonAboveParticle(const CMoonAboveParticle& Particle);
    virtual ~CMoonAboveParticle() override;
private:
    CSharedPtr<class CParticleComponent> m_Particle;
public:
    virtual bool Init() override;
    virtual CMoonAboveParticle* Clone() override;
};

