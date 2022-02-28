#pragma once
#include "GraphicShader.h"

class CTileMapEmptyShader :
    public CGraphicShader
{
    friend class CShaderManager;
private:
    CTileMapEmptyShader();
    CTileMapEmptyShader(const CTileMapEmptyShader& Shader);
    virtual ~CTileMapEmptyShader() override;
public:
    virtual bool Init() override;
};

