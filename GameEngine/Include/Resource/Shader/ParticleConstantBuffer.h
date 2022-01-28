#pragma once
#include "ConstantBufferBase.h"

class CParticleConstantBuffer :
    public CConstantBufferBase
{
public :
    CParticleConstantBuffer();
    CParticleConstantBuffer(const CParticleConstantBuffer& Buffer);
    virtual ~CParticleConstantBuffer() override;
protected:
    ParticleCBuffer m_BufferData;
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CParticleConstantBuffer* Clone() override;
public :
    Vector3 GetStartMin() const
{
        return m_BufferData.StartMin;
}
    Vector3 GetStartMax() const
    {
        return m_BufferData.StartMax;
    }
public :
    void SetSpawnCount(int Count)
{
        m_BufferData.SpawnCountMax = Count;
}
    void SetSpawnEnable(unsigned int Enable)
{
        m_BufferData.SpawnEnable = Enable;
}
    void SetStartMin(const Vector3& StartMin)
    {
        m_BufferData.StartMin = StartMin;
    }
    void SetStartMax(const Vector3& StartMax)
    {
        m_BufferData.StartMax = StartMax;
    }
    void SetSpawnCountMax(unsigned int SpawnCountMax)
    {
        m_BufferData.SpawnCountMax = SpawnCountMax;
    }
    void SetScaleMin(const Vector3& ScaleMin)
    {
        m_BufferData.ScaleMin = ScaleMin;
    }
    void SetLifeTimeMin(float LifeTimeMin)
    {
        m_BufferData.LifeTimeMin = LifeTimeMin;
    }
    void SetScaleMax(const Vector3& ScaleMax)
    {
        m_BufferData.ScaleMax = ScaleMax;
    }
    void SetLifeTimeMax(float LifeTimeMax)
    {
        m_BufferData.LifeTimeMax = LifeTimeMax;
    }
    void SetColorMin(const Vector4& ColorMin)
    {
        m_BufferData.ColorMin = ColorMin;
    }
    void SetColorMax(const Vector4& ColorMax)
    {
        m_BufferData.ColorMax = ColorMax;
    }
    void SetSpeedMin(float SpeedMin)
    {
        m_BufferData.SpeedMin = SpeedMin;
    }
    void SetSpeedMax(float SpeedMax)
    {
        m_BufferData.SpeedMax = SpeedMax;
    }
    void SetMove(int Move)
    {
        m_BufferData.Move = Move;
    }
    void SetGravity(int Gravity)
    {
        m_BufferData.Gravity = Gravity;
    }
    void SetMoveDir(const Vector3& MoveDir)
    {
        m_BufferData.MoveDir = MoveDir;
    }
    void SetIs2D(int Is2D)
    {
        m_BufferData.Is2D = Is2D;
    }
    void SetMoveAngle(const Vector3& MoveAngle)
    {
        m_BufferData.MoveAngle = MoveAngle;
    }
};

