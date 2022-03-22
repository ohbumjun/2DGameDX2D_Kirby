#pragma once

#include "ConstantBufferBase.h"

class CMaterialConstantBuffer :
	public CConstantBufferBase
{
public:
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer);
	virtual ~CMaterialConstantBuffer() override;

protected:
	MaterialCBuffer m_BufferData;

public:
	virtual bool                     Init() override;
	virtual void                     UpdateCBuffer() override;
	virtual CMaterialConstantBuffer* Clone() override;
public :
	float GetOpacity() const
{
		return m_BufferData.Opacity;
}
public:
	void SetPaperBurnEnable(bool Enable)
	{
		m_BufferData.PaperBurnEnable = Enable;
	}
	void SetBaseColor(const Vector4& BaseColor)
	{
		m_BufferData.BaseColor = BaseColor;
	}
	void SetOpacity(float Opacity)
	{
		m_BufferData.Opacity = Opacity;
	}
	void SetPaperBurn(bool Enable)
	{
		m_BufferData.PaperBurnEnable = Enable ? 1 : 0;
	}
};
