#include "BossFightParticle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/ParticleComponent.h"

CBossFightParticle::CBossFightParticle()
{
	SetTypeID<CBossFightParticle>();
}

CBossFightParticle::~CBossFightParticle()
{}

bool CBossFightParticle::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	// m_Particle->SetRelativePos(200.f, 50.f, 0.f);

	m_Particle->SetParticle("BossFight");
	// m_Particle->SetParticle("Bubble");

	return true;
}
