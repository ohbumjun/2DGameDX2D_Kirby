#include "Shader.h"

CShader::CShader()
{
}

CShader::CShader(const CShader& Shader)
{
	*this = Shader;
	m_RefCount = 0;
}

CShader::~CShader()
{
}
