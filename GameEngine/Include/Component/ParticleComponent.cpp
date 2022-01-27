#include "ParticleComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Scene/Scene.h"

CParticleComponent::CParticleComponent() :
m_SpawnTime(0.f),
m_SpawnTimeMax(0.1f),
m_ParticleInfo{},
m_ParticleSharedInfo{},
m_ParticleConstantBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;
}

CParticleComponent::CParticleComponent(const CParticleComponent& Component)
{
	m_SpawnTime = 0.f;
	m_SpawnTimeMax = Component.m_SpawnTimeMax;

	// Particle Resource�� �����ؼ� ����� ���̴�.
	m_Particle = Component.m_Particle;

	size_t BufferCount = m_vecStructuredBuffer.size();

	// ����ȭ ���۴� ���� ���縦 ����� �Ѵ�.
	// ������ ���� Ȥ�ó��� ���� ������ �ִ� StructuredBuffer�� ��� �����ش�.
	for (size_t i = 0; i < BufferCount; i++)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	m_vecStructuredBuffer.clear();

	if (m_Particle)
	{
		// Particle Component�� ��� �ִ� ��������� Particle Resource�κ��� ���� ���̴�.
		m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

		// Update Shader�� ��������
		m_UpdateShader = m_Particle->GetParticleUpdateShader();

		SAFE_DELETE(m_ParticleConstantBuffer);

		m_ParticleConstantBuffer = m_Particle->CloneParticleConstantBuffer();
	}

}

CParticleComponent::~CParticleComponent()
{
	// ����ȭ ���� �����ֱ�
	size_t  Size = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	// ��� ���� �����ֱ�
	SAFE_DELETE(m_ParticleConstantBuffer);
}

void CParticleComponent::SetParticle(const std::string& Name)
{
	SetParticle(m_Scene->GetResource()->FindParticle(Name));
}

void CParticleComponent::SetParticle(CParticle* Particle)
{}

void CParticleComponent::SetSpawnTime(float Time)
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
