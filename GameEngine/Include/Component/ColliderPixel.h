#pragma once
#include "ColliderComponent.h"

class CColliderPixel :
    public CColliderComponent
{
    friend class CGameObject;
protected :
    CColliderPixel();
    CColliderPixel(const CColliderPixel* Pixel);
    virtual ~CColliderPixel() override;
protected :
    PixelInfo m_Info;
    CSharedPtr<class CMesh> m_PixelMesh;
    CShaderPtr<class CShader> m_PixelShader;
};

