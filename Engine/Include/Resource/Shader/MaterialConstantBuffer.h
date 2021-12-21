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

public:
	void SetBaseColor(const Vector4& BaseColor)
	{
		m_BufferData.BaseColor = BaseColor;
	}

	void SetOpacity(float Opacity)
	{
		m_BufferData.Opacity = Opacity;
	}
};
