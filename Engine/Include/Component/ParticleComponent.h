#pragma once

#include "SceneComponent.h"
#include "../../Resource/Particle/Particle.h"
#include "../../Resource/Mesh/Mesh.h"
#include "../../Resource/Particle/ParticleUpdateShader.h"
#include "../../Resource/Shader/ParticleConstantBuffer.h"
#include "../../Resource/Shader/StructuredBuffer.h"

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
protected :
    CSharedPtr<CParticle> m_Particle;
    CSharedPtr<CMaterial> m_Material;
    CParticleConstantBuffer* m_ParticleConstantBuffer;
    std::vector<CStructuredBuffer*> m_vecStructruedBuffer;
    CSharedPtr<CParticleUpdateShader> m_ParticleUpdateShader;
    ParticleInfo m_ParticleInfo;
    ParticleInfoShared m_ParticleInfoShared;
private :
    CSharedPtr<CMesh> m_Mesh; // Particle은 Mesh가 없다. Particle Component가 가지고 있게 되는 것
    float m_SpawnTime;
    float m_SpawnTimeMax;
public :
    void SetParticle(const std::string& Name);
    void SetParticle(const CParticle* Particle);
    void SetSpawnTime(float Time);
public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CParticleComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

