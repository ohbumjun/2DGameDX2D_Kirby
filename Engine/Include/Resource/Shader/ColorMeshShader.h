#pragma once
#include "GraphicShader.h"

class CColorMeshShader :
	public CGraphicShader
{
	friend class CShaderManager;

protected:
	CColorMeshShader();
	virtual ~CColorMeshShader() override;

public:
	virtual bool Init() override;
};
