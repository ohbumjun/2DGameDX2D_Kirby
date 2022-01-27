#pragma once
#include "../../Resource/Shader/GraphicShader.h"

class CParticleRenderShader :
    public CGraphicShader
{
    friend class CShaderManager;
private :
    CParticleRenderShader();
    ~CParticleRenderShader();
public :
    virtual bool Init() override;
};

