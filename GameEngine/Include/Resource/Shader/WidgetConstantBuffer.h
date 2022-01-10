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
        m_BufferData.matWVP = matWP;
}
    void SetUseTexture(bool Use)
{
        m_BufferData.UseTexture = Use ? 1 : 0;
}
};

