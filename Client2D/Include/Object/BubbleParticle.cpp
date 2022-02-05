#include "BubbleParticle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/ParticleComponent.h"

CBubbleParticle::CBubbleParticle()
{
	SetTypeID<CBubbleParticle>();
}

CBubbleParticle::CBubbleParticle(const CBubbleParticle& Particle) :
CGameObject(Particle)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CBubbleParticle::~CBubbleParticle()
{}

bool CBubbleParticle::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	m_Particle->SetRelativePos(200.f, 50.f, 0.f);

	m_Particle->SetParticle("Bubble");

	// SetLifeTime(5.f);

	return true;
}

CBubbleParticle* CBubbleParticle::Clone()
{
	return new CBubbleParticle(*this);
}
