#pragma once
#include "C:\Users\dhsys\Desktop\DX_Project\Engine\Include\GameObject\GameObject.h"

class CSpecialChangeParticle :
    public CGameObject
{
    friend class CScene;
protected:
    CSpecialChangeParticle();
    CSpecialChangeParticle(const CSpecialChangeParticle& Particle);
    virtual ~CSpecialChangeParticle() override;
private:
    CSharedPtr<class CParticleComponent> m_Particle;
public:
    virtual bool Init() override;
    virtual CSpecialChangeParticle* Clone() override;
};

