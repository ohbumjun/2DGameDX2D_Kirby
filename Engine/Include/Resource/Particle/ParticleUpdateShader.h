#pragma once
#include "../Shader/ComputeShader.h"

class CParticleUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;
private :
    CParticleUpdateShader();
    CParticleUpdateShader(const CParticleUpdateShader& Shader);
    virtual ~CParticleUpdateShader() override;
public:
    virtual bool Init() override;
    virtual void SetShader() override;
    virtual CParticleUpdateShader* Clone() override;
};

