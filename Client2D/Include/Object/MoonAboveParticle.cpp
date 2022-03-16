#include "MoonAboveParticle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/ParticleComponent.h"

CMoonAboveParticle::CMoonAboveParticle()
{
	SetTypeID<CMoonAboveParticle>();
}

CMoonAboveParticle::CMoonAboveParticle(const CMoonAboveParticle& Particle) :
	CGameObject(Particle)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CMoonAboveParticle::~CMoonAboveParticle()
{}

bool CMoonAboveParticle::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	// m_Particle->SetRelativePos(200.f, 50.f, 0.f);

	m_Particle->SetParticle("MoonAbove");

	// SetLifeTime(5.f);

	return true;
}

CMoonAboveParticle* CMoonAboveParticle::Clone()
{
	return new CMoonAboveParticle(*this);
}
