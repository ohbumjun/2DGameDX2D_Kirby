#include "Particle.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/StructuredBuffer.h"
#include "../Particle/ParticleUpdateShader.h"

CParticle::CParticle() :
	m_ParticleConstantBuffer(nullptr),
	m_SpawnTime(0.f),
	m_Is2D(true)
{}

CParticle::CParticle(const CParticle& Particle) : CRef(Particle)
{
	m_ParticleConstantBuffer = Particle.m_ParticleConstantBuffer->Clone();
	m_ParticleUpdateShader = Particle.m_ParticleUpdateShader;

}

CParticle::~CParticle()
{
	SAFE_DELETE(m_ParticleConstantBuffer);
}

bool CParticle::Init()
{
	m_ParticleConstantBuffer = new CParticleConstantBuffer;

	if (!m_ParticleConstantBuffer->Init())
		return false;

	if (m_Scene)
	{
		m_ParticleUpdateShader = (CParticleUpdateShader*)m_Scene->GetResource()->FindShader("ParticleUpdateShader");
	}
	else
	{
		m_ParticleUpdateShader = (CParticleUpdateShader*)CResourceManager::GetInst()->FindShader("ParticleUpdateShader");
	}
	return true;
}

void CParticle::AddStructuredBuffer(const std::string& Name, int Register, int Size, bool Dynamic, int Count,
									int                StructuredBufferShaderType)
{
	size_t VecSize = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < VecSize; i++)
	{
		if (m_vecStructuredBuffer[i]->m_Name == Name)
			return;
	}

	CStructuredBuffer* Buffer = new CStructuredBuffer;
	if (!Buffer->Init(Name, Register,  Size, Dynamic, Count, StructuredBufferShaderType))
	{
		SAFE_DELETE(Buffer);
		return;
	}

	m_vecStructuredBuffer.push_back(Buffer);
}
