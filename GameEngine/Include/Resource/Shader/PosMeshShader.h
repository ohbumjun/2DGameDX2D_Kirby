#pragma once
#include "GraphicShader.h"

class CPosMeshShader :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CPosMeshShader();
	virtual ~CPosMeshShader() override;
public :
	virtual bool Init() override;
};
