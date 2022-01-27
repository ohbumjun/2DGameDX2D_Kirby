#pragma once

#include "SceneComponent.h"
#include "../../Resource/Particle/Particle.h"
#include "../../Resource/Mesh/Mesh.h"
#include "../../Resource/Particle/ParticleUpdateShader.h"
#include "../../Resource/Shader/ParticleConstantBuffer.h"

// ParticleComponent�� �뵵�� �����ϱ� ?
// �� �ȿ� ���� ���ϴ� Particle Resource�� �����ΰ�,
// Particle Component �� ���� ������ �ð� ���� �����ؼ� ����ϴ� ���̴�. 

class CParticleComponent :
    public CSceneComponent
{
public :
    CParticleComponent();
    CParticleComponent(const CParticleComponent& Component);
    virtual ~CParticleComponent();
private :
	// � Particle Resource�� ����ϴ��� �ݵ�� ��� �־�� �Ѵ�.
	CSharedPtr<CParticle> m_Particle;
	// �ش� Particle Component�� ��� �ְ� �ϱ� ���� Particle Mesh, Shader�� �ʿ��ϴ�
	// Mesh�� ���, �� �ϳ��� --> ���� ���̴����� �� �� �ϳ��� Ȯ���ؼ� �簢���� ���� ���̴�
	CSharedPtr<CMesh> m_Mesh;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	// �׿� �켱 Particle.h �� ������ ������ ����
	std::vector<class CStructuredBuffer*> m_vecStructuredBuffer;
	CParticleConstantBuffer* m_ParticleConstantBuffer;
	ParticleInfo m_ParticleInfo;
	ParticleInfoShared m_ParticleSharedInfo;
	float m_SpawnTime;
	// 
	float m_SpawnTimeMax;
	
public:
	virtual void             Start() override;
	virtual bool             Init() override;
	virtual void             Update(float DeltaTime) override;
	virtual void             PostUpdate(float DeltaTime) override;
	virtual void             PrevRender() override;
	virtual void             Render() override;
	virtual void CheckCollision();
	virtual void             PostRender() override;
	virtual CParticleComponent* Clone() override;
};

