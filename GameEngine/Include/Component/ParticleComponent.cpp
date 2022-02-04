#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CParticleComponent::CParticleComponent() :
m_SpawnTimeMax(0.01f),
m_SpawnTime(0.0f),
m_ParticleInfo{},
m_ParticleInfoShared{},
m_ParticleConstantBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;
}

CParticleComponent::CParticleComponent(const CParticleComponent& Component)
{
	m_SpawnTime = 0.f;
	m_SpawnTimeMax = Component.m_SpawnTimeMax;

	m_Particle = Component.m_Particle;

	size_t Size = m_vecStructruedBuffer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecStructruedBuffer[i]);
	}

	m_vecStructruedBuffer.clear();

	if (m_Particle)
	{
		m_Particle->CloneStructuredBuffer(m_vecStructruedBuffer);

		m_ParticleUpdateShader = m_Particle->GetParticleUpdateShader();

		SAFE_DELETE(m_ParticleConstantBuffer);

		m_ParticleConstantBuffer = m_Particle->CloneParticleConstantBuffer();
	}

}

CParticleComponent::~CParticleComponent()
{
	size_t Size = m_vecStructruedBuffer.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecStructruedBuffer[i]);
	}

	SAFE_DELETE(m_ParticleConstantBuffer);
}

void CParticleComponent::SetParticle(const std::string& Name)
{
	SetParticle(m_Scene->GetResource()->FindParticle(Name));
}

void CParticleComponent::SetParticle(const CParticle* Particle)
{
	if (!Particle)
		return;

	m_Particle = const_cast<CParticle*>(Particle);

	if (m_Particle)
	{
		size_t Size = m_vecStructruedBuffer.size();

		for (size_t i = 0; i < Size; i++)
		{
			SAFE_DELETE(m_vecStructruedBuffer[i]);
		}

		m_vecStructruedBuffer.clear();

		m_Particle->CloneStructuredBuffer(m_vecStructruedBuffer);

		SAFE_DELETE(m_ParticleConstantBuffer);

		m_ParticleConstantBuffer = m_Particle->CloneParticleConstantBuffer();

		m_Material = m_Particle->CloneMaterial();

		m_ParticleUpdateShader = m_Particle->GetParticleUpdateShader();

		m_SpawnTimeMax = m_Particle->GetSpawnTime();
	}

}

void CParticleComponent::SetSpawnTime(float Time)
{
	m_SpawnTimeMax = Time;
	m_Particle->SetSpawnTime(Time);
}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

bool CParticleComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_Mesh = m_Scene->GetResource()->FindMesh("ParticlePointMesh");

	if (!m_Mesh)
		return false;

	return true;
}

void CParticleComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	m_SpawnTime += DeltaTime;

	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime -= m_SpawnTimeMax;

		m_ParticleConstantBuffer->SetSpawnEnable(1);
	}
	else
	{
		m_ParticleConstantBuffer->SetSpawnEnable(0);
	}
}

void CParticleComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	// �ǽð� ��ġ ������ ���� �Ÿ� �����Ѵ�.
	CParticleConstantBuffer* Buffer = m_Particle->GetParticleConstantBuffer(); // ��� ������ ���� ex) -1, 1

	Vector3 StartMin, StartMax;

	StartMin = GetWorldPos() + Buffer->GetStartMin(); 
	StartMax = GetWorldPos() + Buffer->GetStartMax();

	m_ParticleConstantBuffer->SetStartMin(StartMin); // ���� ��ġ ���� ����
	m_ParticleConstantBuffer->SetStartMax(StartMax);

	m_ParticleConstantBuffer->UpdateCBuffer();

	// Compute Shader --> Update Shader�� ���۽�Ų��.

	size_t Bufferize = m_vecStructruedBuffer.size();

	for (size_t i = 0; i < Bufferize; i++)
	{
		m_vecStructruedBuffer[i]->SetShader();
	}

	int GroupCount = m_Particle->GetSpawnCountMax() / 64 + 1;
	m_ParticleUpdateShader->Execute(GroupCount, 1, 1);

	// ����ȭ ���� �������ֱ�
	for (size_t i = 0; i < Bufferize; i++)
	{
		m_vecStructruedBuffer[i]->ResetShader();
	}
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::Render()
{
	CSceneComponent::Render();

	size_t Size = m_vecStructruedBuffer.size();

	// ���� ���̴��� ���� �� �ִ� �б� ���� ����ȭ ���ۿ� �Ѱ��� ���̴�.
	for (size_t i = 0; i < Size; i++)
	{
		m_vecStructruedBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	// Material�� �׸���
	if (m_Material)
		m_Material->Render();

	// Mesh�� �ν��Ͻ��� �̿��ؼ� �׸���.
	int SpawnCount = m_ParticleConstantBuffer->GetSpawnCountMax();
	m_Mesh->RenderInstancing(SpawnCount);

	for (size_t i = 0; i < Size; i++)
	{
		m_vecStructruedBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	// Material�� �׸���
	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}

void CParticleComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);
}

void CParticleComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}
