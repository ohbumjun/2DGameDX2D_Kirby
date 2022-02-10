#pragma once
#include "ConstantBufferBase.h"
class CTileConstantBuffer :
    public CConstantBufferBase
{
    friend class CShaderManager;
private :
    CTileConstantBuffer();
    CTileConstantBuffer(const CTileConstantBuffer& Buffer);
    virtual ~CTileConstantBuffer() override;
private :
    TileCBuffer m_BufferData;
public :
    void SetImageSize(const Vector2& ImageSize)
{
        m_BufferData.ImageSize = ImageSize;
}
    void SetTileStartUV(const Vector2& Start);
    void SetTileEndUV(const Vector2& End);
    void SetTileSize(const Vector2& Size);
    void SetTileImageSize(const Vector2& Size);
    void SetMatWVP(const Matrix& matWVP);
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
};

