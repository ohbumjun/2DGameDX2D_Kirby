#pragma once
#include "../../Ref.h"
#include "../Shader/ParticleConstantBuffer.h"
#include "../../Resource/Material/Material.h"

class CParticle :
    public CRef
{
    friend class CParticleManager;
private :
    CParticle();
    CParticle(const CParticle& Particle);
    virtual ~CParticle() override;
private :
    class CScene* m_Scene;
    std::vector<class CStructuredBuffer*> m_vecStructuredBuffer;
    CSharedPtr<class CMaterial> m_Material;
    CSharedPtr<class CParticleUpdateShader> m_ParticleUpdateShader;
    ParticleInfo m_Info;
    ParticleInfoShared m_InfoShared;
    class CParticleConstantBuffer* m_ParticleConstantBuffer;
    float m_SpawnTime;
    bool m_Is2D;
    int   m_SpawnCountMax;
public :
    bool Init();
public:
    void AddStructuredBuffer(const std::string& Name, int Register, int Size, int Count, 
        bool Dynamic = false,
        int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
    void ResizeBuffer(const std::string& Name, unsigned int Size, unsigned int Count,
        int Register, bool Dynamic = false,
        int StructuredBufferShaderType = (int)Buffer_Shader_Type::Compute);
    void CloneStructuredBuffer(std::vector<class CStructuredBuffer*>& vecBuffer);
public :
    CParticleUpdateShader* GetParticleUpdateShader() const
{
        return m_ParticleUpdateShader;
}
    CParticleConstantBuffer* GetParticleConstantBuffer() const
{
        return m_ParticleConstantBuffer;
}
    CParticleConstantBuffer* CloneParticleConstantBuffer() const
{
        return m_ParticleConstantBuffer->Clone();
}
    float GetSpawnTime() const
{
        return m_SpawnTime;
}
    int GetSpawnCountMax() const
{
        return m_SpawnCountMax;
}
    CMaterial* CloneMaterial() const
    {
        return m_Material;
    }
public : // 상수 정보 세팅
    void SetSpawnCountMax(int Count);
    void SetSpawnTime(float Time)
    {
        m_SpawnTime = Time;
    }
    void SetMaterial(class CMaterial* Material)
    {
        m_Material = Material;
    }
    void SetSpawnCount(unsigned int Count)
    {
        m_ParticleConstantBuffer->SetSpawnCount(Count);
    }
    void SetStartMin(const Vector3& StartMin)
    {
        m_ParticleConstantBuffer->SetStartMin(StartMin);
    }
    void SetStartMax(const Vector3& StartMax)
    {
        m_ParticleConstantBuffer->SetStartMax(StartMax);
    }
    void SetSpawnCountMax(unsigned int SpawnCountMax)
    {
        m_ParticleConstantBuffer->SetSpawnCountMax(SpawnCountMax);
    }
    void SetScaleMin(const Vector3& ScaleMin)
    {
        m_ParticleConstantBuffer->SetScaleMin(ScaleMin);
    }
    void SetLifeTimeMin(float LifeTimeMin)
    {
        m_ParticleConstantBuffer->SetLifeTimeMin(LifeTimeMin);
    }
    void SetScaleMax(const Vector3& ScaleMax)
    {
        m_ParticleConstantBuffer->SetScaleMax(ScaleMax);
    }
    void SetLifeTimeMax(float LifeTimeMax)
    {
        m_ParticleConstantBuffer->SetLifeTimeMax(LifeTimeMax);
    }
    void SetColorMin(const Vector4& ColorMin)
    {
        m_ParticleConstantBuffer->SetColorMin(ColorMin);
    }
    void SetColorMax(const Vector4& ColorMax)
    {
        m_ParticleConstantBuffer->SetColorMax(ColorMax);
    }
    void SetSpeedMin(float SpeedMin)
    {
        m_ParticleConstantBuffer->SetSpeedMin(SpeedMin);
    }
    void SetSpeedMax(float SpeedMax)
    {
        m_ParticleConstantBuffer->SetSpeedMax(SpeedMax);
    }
    void SetMove(int Move)
    {
        m_ParticleConstantBuffer->SetMove(Move);
    }
    void SetGravity(int Gravity)
    {
        m_ParticleConstantBuffer->SetGravity(Gravity);
    }
    void SetMoveDir(const Vector3& MoveDir)
    {
        m_ParticleConstantBuffer->SetMoveDir(MoveDir);
    }
    void SetIs2D(int Is2D)
    {
        m_ParticleConstantBuffer->SetIs2D(Is2D);
    }
    void SetMoveAngle(const Vector3& MoveAngle)
    {
        m_ParticleConstantBuffer->SetMoveAngle(MoveAngle);
    }
};


