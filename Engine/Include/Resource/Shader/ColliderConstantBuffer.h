#pragma once

#include "ConstantBufferBase.h"

class CColliderConstantBuffer :
public CConstantBufferBase
{
public :
	CColliderConstantBuffer();
	CColliderConstantBuffer(const CColliderConstantBuffer& Buffer);
	~CColliderConstantBuffer();
protected :
	ColliderCBuffer m_BufferData;
public :
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CColliderConstantBuffer* Clone();
public :
	void SetColliderColor(const Vector4& Color)
{
		m_BufferData.Color = Color;
}
	void SetColliderColor(float r, float g, float b, float a = 1.f)
	{
		m_BufferData.Color = Vector4(r,g,b,a);
	}
	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
	}
};

