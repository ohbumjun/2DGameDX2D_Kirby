#include "Particle.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/StructuredBuffer.h"
#include "../Particle/ParticleUpdateShader.h"

CParticle::CParticle() :
	m_ParticleConstantBuffer(nullptr),
	m_SpawnTime(0.f),
	m_Is2D(true),
	m_SpawnCountMax(100)
{}

CParticle::CParticle(const CParticle& Particle) : CRef(Particle)
{
	m_ParticleConstantBuffer = Particle.m_ParticleConstantBuffer->Clone();
	m_ParticleUpdateShader = Particle.m_ParticleUpdateShader;

}

CParticle::~CParticle()
{
	SAFE_DELETE(m_ParticleConstantBuffer);

	size_t Size = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}
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

	// 구조화 버퍼에 들어가는 구조체들을 이용하여, 구조화 버퍼를 만들 것이다.
	// 파티클의 개수만큼 만들 것이다. 최대 생성할 파티클 정보는 m_SpawnCountMax 변수에 저장한다.
	AddStructuredBuffer("ParticleInfo", 0, m_SpawnCountMax, sizeof(ParticleInfo));

	// 공용 구조체 버퍼의 경우, 단 1개만을 만들 것이다.
	AddStructuredBuffer("ParticleInfoShared", 1, 1, sizeof(ParticleInfoShared));

	return true;
}

void CParticle::AddStructuredBuffer(const std::string& Name, int Register, int Count, int Size, bool Dynamic,
									int                StructuredBufferShaderType)
{
	size_t VecSize = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < VecSize; i++)
	{
		if (m_vecStructuredBuffer[i]->GetName() == Name)
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

void CParticle::ResizeBuffer(const std::string& Name, unsigned Size, unsigned Count, int Register, bool Dynamic,
	int StructuredBufferShaderType)
{
	size_t vecSize = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < vecSize; i++)
	{
		if (m_vecStructuredBuffer[i]->GetName() == Name)
		{
			m_vecStructuredBuffer[i]->Init(Name, Register, Size, Dynamic, Count, StructuredBufferShaderType);
			break;
		}
	}
}

void CParticle::CloneStructuredBuffer(std::vector<CStructuredBuffer*>& vecBuffer)
{
	size_t Size = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < Size; i++)
	{
		vecBuffer.push_back(m_vecStructuredBuffer[i]->Clone());
	}
}

void CParticle::SetSpawnCountMax(int Count)
{
	// 최대 생성할 파티클 개수 정보 수정
	m_SpawnCountMax = Count;

	m_ParticleConstantBuffer->SetSpawnCountMax(Count);

	ResizeBuffer("ParticleInfo", sizeof(ParticleInfo), m_SpawnCountMax, 0);
}
