#pragma once
#include "ConstantBufferBase.h"

class CPaperBurnConstantBuffer :
    public CConstantBufferBase
{
    friend class CPaperBurnComponent;
private :
    CPaperBurnConstantBuffer();
    CPaperBurnConstantBuffer(const CPaperBurnConstantBuffer& Buffer);
    virtual ~CPaperBurnConstantBuffer() override;
private :
    ParticleCBuffer m_BufferData;
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CPaperBurnConstantBuffer* Clone() override;
};

