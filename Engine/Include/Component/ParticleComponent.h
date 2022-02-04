#pragma once

#include "SceneComponent.h"
#include "../../Resource/Particle/Particle.h"
#include "../../Resource/Mesh/Mesh.h"
#include "../../Resource/Particle/ParticleUpdateShader.h"
#include "../../Resource/Shader/ParticleConstantBuffer.h"
#include "../../Resource/Shader/StructuredBuffer.h"

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
protected :
    CSharedPtr<CParticle> m_Particle;
    CSharedPtr<CMaterial> m_Material;
    CParticleConstantBuffer* m_ParticleConstantBuffer;
    std::vector<CStructuredBuffer*> m_vecStructruedBuffer;
    CSharedPtr<CParticleUpdateShader> m_ParticleUpdateShader;
    ParticleInfo m_ParticleInfo;
    ParticleInfoShared m_ParticleInfoShared;
private :
    CSharedPtr<CMesh> m_Mesh; // Particle�� Mesh�� ����. Particle Component�� ������ �ְ� �Ǵ� ��
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

