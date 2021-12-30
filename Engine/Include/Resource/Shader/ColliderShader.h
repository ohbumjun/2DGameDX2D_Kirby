#pragma once

#include "GraphicShader.h"

class CColliderShader :
    public CGraphicShader
{
    friend class CShaderManager;
protected :
    CColliderShader();
    ~CColliderShader();
public :
    virtual bool Init();
};


