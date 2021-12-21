#pragma once
#include "Shader.h"

class CComputeShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CComputeShader();
	virtual ~CComputeShader() override = 0;

public:
	virtual bool Init() override = 0;
	virtual void SetShader() override;
};
