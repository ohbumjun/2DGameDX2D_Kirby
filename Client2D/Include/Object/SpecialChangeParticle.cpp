#include "SpecialChangeParticle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/ParticleComponent.h"

CSpecialChangeParticle::CSpecialChangeParticle()
{
	SetTypeID<CSpecialChangeParticle>();
}

CSpecialChangeParticle::CSpecialChangeParticle(const CSpecialChangeParticle& Particle) :
	CGameObject(Particle)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CSpecialChangeParticle::~CSpecialChangeParticle()
{}

bool CSpecialChangeParticle::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	// m_Particle->SetRelativePos(200.f, 50.f, 0.f);

	m_Particle->SetParticle("SpecialChange");

	SetLifeTime(6.f);

	return true;
}

CSpecialChangeParticle* CSpecialChangeParticle::Clone()
{
	return new CSpecialChangeParticle(*this);
}
