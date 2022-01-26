#include "ParticleUpdateShader.h"

CParticleUpdateShader::CParticleUpdateShader()
{}

CParticleUpdateShader::CParticleUpdateShader(const CParticleUpdateShader& Shader)
{}

CParticleUpdateShader::~CParticleUpdateShader()
{}

bool CParticleUpdateShader::Init()
{
	if (!Load)
	return true;
}

void CParticleUpdateShader::SetShader()
{
	CComputeShader::SetShader();
}
