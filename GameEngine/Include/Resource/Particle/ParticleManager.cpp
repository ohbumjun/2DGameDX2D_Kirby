#include "ParticleManager.h"
#include "Particle.h"

CParticleManager::CParticleManager()
{}

CParticleManager::~CParticleManager()
{}

bool CParticleManager::Init()
{
	return true;
}

bool CParticleManager::CreateParticle(const std::string& Name)
{
	CParticle* Particle = FindParticle(Name);
	if (Particle)
		return true;

	Particle = new CParticle;
	if (!Particle->Init())
	{
		SAFE_DELETE(Particle);
		return false;
	}

	m_mapParticle.insert(std::make_pair(Name, Particle));

	return true;
}

CParticle* CParticleManager::FindParticle(const std::string& Name)
{
	auto iter = m_mapParticle.find(Name);

	if (iter == m_mapParticle.end())
		return nullptr;

	return iter->second;
}
