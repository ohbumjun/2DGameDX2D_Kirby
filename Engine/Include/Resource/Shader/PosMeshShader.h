#pragma once
#include "GraphicShader.h"
class CPosMeshShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CPosMeshShader();
    ~CPosMeshShader();
public :
    virtual bool Init() override;
};

