#pragma once
#include "GameObject\GameObject.h"

class CBubbleParticle :
    public CGameObject
{
    friend class CScene;
protected:
    CBubbleParticle();
    CBubbleParticle(const CBubbleParticle& Particle);
    virtual ~CBubbleParticle() override;
private :
    CSharedPtr<class CParticleComponent> m_Particle;
public :
    virtual bool Init() override;
    virtual CBubbleParticle* Clone() override;
};

