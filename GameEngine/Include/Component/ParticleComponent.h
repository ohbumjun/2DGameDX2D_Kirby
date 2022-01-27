#pragma once

#include "SceneComponent.h"
#include "../../Resource/Particle/Particle.h"
#include "../../Resource/Mesh/Mesh.h"
#include "../../Resource/Particle/ParticleUpdateShader.h"
#include "../../Resource/Shader/ParticleConstantBuffer.h"

// ParticleComponent의 용도는 무엇일까 ?
// 이 안에 내가 원하는 Particle Resource를 만들어두고,
// Particle Component 를 통해 나오는 시간 등을 조절해서 사용하는 것이다. 

class CParticleComponent :
    public CSceneComponent
{
public :
    CParticleComponent();
    CParticleComponent(const CParticleComponent& Component);
    virtual ~CParticleComponent();
private :
	// 어떤 Particle Resource를 사용하는지 반드시 들고 있어야 한다.
	CSharedPtr<CParticle> m_Particle;
	// 해당 Particle Component를 들고 있게 하기 위한 Particle Mesh, Shader가 필요하다
	// Mesh의 경우, 점 하나만 --> 기하 셰이더에서 이 점 하나를 확장해서 사각형을 만들어낼 것이다
	CSharedPtr<CMesh> m_Mesh;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	// 그외 우선 Particle.h 와 동일한 정보들 세팅
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

