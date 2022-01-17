#pragma once
#include "ConstantBufferBase.h"
class CProgressbarConstantBuffer :
    public CConstantBufferBase
{
public :
    CProgressbarConstantBuffer();
    CProgressbarConstantBuffer(const CProgressbarConstantBuffer& Buffer);
    virtual ~CProgressbarConstantBuffer() override;
private :
    ProgressBarCBuffer m_BufferData;
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CProgressbarConstantBuffer* Clone() override;
public :
    void SetPercent(float Percent)
{
        m_BufferData.Percent = Percent;
}
    void SetDir(ProgressBar_Dir Dir)
{
        m_BufferData.Dir = (int)Dir;
}
};

