#pragma once
#include "ConstantBufferBase.h"

class CWidgetConstantBuffer :
    public CConstantBufferBase
{
public :
    CWidgetConstantBuffer();
    CWidgetConstantBuffer(const CWidgetConstantBuffer& Widget);
    virtual ~CWidgetConstantBuffer() override;
private :
    WidgetCBuffer m_BufferData;
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CWidgetConstantBuffer* Clone();
public :
    void SetTint(const Vector4& Color)
{
        m_BufferData.Tint = Color;
}
    void SetWP(const Matrix& matWP)
{
        m_BufferData.matWP = matWP;
}
    void SetUseAnimation(bool Use)
{
        m_BufferData.WidgetAnimEnable = Use ? 1 : 0;
}
    void SetUseTexture(bool Use)
{
        m_BufferData.UseTexture = Use ? 1 : 0;
}
    void SetAnimType(Image_Type Type)
{
        m_BufferData.WidgetAnimType = (int)Type;
}
    void SetStartUV(const Vector2& StartUV)
{
        m_BufferData.Animation2DStartUV = StartUV;
}
    void SetEndUV(const Vector2& EndUV)
    {
        m_BufferData.Animation2DEndUV = EndUV;
    }
};

