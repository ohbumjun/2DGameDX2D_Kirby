#include "ParticleComponent.h"

CParticleComponent::CParticleComponent()
{}

CParticleComponent::CParticleComponent(const CParticleComponent& Component)
{}

CParticleComponent::~CParticleComponent()
{}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

bool CParticleComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CParticleComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CParticleComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::Render()
{
	CSceneComponent::Render();
}

void CParticleComponent::CheckCollision()
{
	CSceneComponent::CheckCollision();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}
