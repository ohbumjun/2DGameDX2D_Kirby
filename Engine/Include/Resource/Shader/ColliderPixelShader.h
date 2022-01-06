#pragma once
#include "GraphicShader.h"

class CColliderPixelShader :
    public CGraphicShader
{
public :
    CColliderPixelShader();
    virtual ~CColliderPixelShader() override;
public :
    virtual bool Init() override;
};

