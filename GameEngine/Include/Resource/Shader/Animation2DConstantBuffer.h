#pragma once

#include "ConstantBufferBase.h"

class CAnimation2DConstantBuffer :
	public CConstantBufferBase
{
public:
	CAnimation2DConstantBuffer();
	CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& Buffer);
	virtual ~CAnimation2DConstantBuffer() override;

protected:
	Animation2DCBuffer m_BufferData;

public:
	virtual bool                        Init() override;
	virtual void                        UpdateCBuffer() override;
	virtual CAnimation2DConstantBuffer* Clone() override;

public:
	void SetAnimation2DCBuffer(Animation2DCBuffer BufferData)
	{
		m_BufferData = BufferData;
	}
	void SetAnimation2DType(Image_Type Type)
	{
		m_BufferData.Animation2DType = static_cast<int>(Type);
	}

	void SetStartUV(const Vector2& StartUV)
	{
		m_BufferData.Animation2DStartUV = StartUV;
	}

	void SetEndUV(const Vector2& EndUV)
	{
		m_BufferData.Animation2DEndUV = EndUV;
	}
public :
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};
