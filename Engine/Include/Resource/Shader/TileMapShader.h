#pragma once
#include "GraphicShader.h"

class CTileMapShader :
    public CGraphicShader
{
    friend class CShaderManager;
private :
    CTileMapShader();
    CTileMapShader(const CTileMapShader& Shader);
    virtual ~CTileMapShader() override;
public :
    virtual bool Init() override;
    
};

