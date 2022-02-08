#pragma once
#include "ConstantBufferBase.h"

class CPaperBurnConstantBuffer :
    public CConstantBufferBase
{
    friend class CPaperBurnComponent;
private :
    CPaperBurnConstantBuffer();
    CPaperBurnConstantBuffer(const CPaperBurnConstantBuffer& Buffer);
    virtual ~CPaperBurnConstantBuffer() override;
private :
    PaperBurnCBuffer m_BufferData;
public :
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CPaperBurnConstantBuffer* Clone() override;
public :
    void SetInLineColor(const Vector4& Color)
{
        m_BufferData.InLineColor = Color;
}
    void SetInLineColor(float r, float g, float b, float a)
{
        m_BufferData.InLineColor = Vector4(r,g,b,a);
}
    void SetCenterLineColor(const Vector4& Color)
{
        m_BufferData.CenterLineColor = Color;
}
    void SetCenterLineColor(float r, float g, float b, float a)
    {
        m_BufferData.CenterLineColor = Vector4(r, g, b, a);;
    }
    void SetOutLineColor(const Vector4& Color)
    {
        m_BufferData.OutLineColor = Color;
    }
    void SetOutLineColor(float r, float g, float b, float a)
    {
        m_BufferData.CenterLineColor = Vector4(r, g, b, a);;
    }
    void SetInverse(bool Enable)
	{
        m_BufferData.Inverse = Enable ? 1 : 0;
	}
    void SetInFilter(float Filter)
	{
        m_BufferData.InFilter = Filter;
	}
    void SetCenterFilter(float Filter)
    {
        m_BufferData.CenterFilter = Filter;
    }
    void SetOutFilter(float Filter)
    {
        m_BufferData.OutFilter = Filter;
    }
    void SetFilter(float Filter)
{
        m_BufferData.Filter = Filter;
}
};

