#include "ParticleRenderShader.h"

CParticleRenderShader::CParticleRenderShader()
{}

CParticleRenderShader::~CParticleRenderShader()
{}

bool CParticleRenderShader::Init()
{
	// 정점 셰이더
	if (!LoadVertexShader("ParticleVS", TEXT("Particle.fx"), SHADER_PATH))
		return false;

	// 기하 셰이더
	if (!LoadGeometryShader("ParticleGS", TEXT("Particle.fx"), SHADER_PATH))
		return false;

	// 픽셀 셰이더
	if (!LoadPixelShader("ParticlePS", TEXT("Particle.fx"), SHADER_PATH))
		return false;

	// 정점은 점 하나만 넘긴다
	// 기하 셰이더에서 해당 점 하나를 사각형으로 확장할 것이다.
	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 
		0, 12, D3D11_INPUT_PER_VERTEX_DATA,0);

	if (!CreateInputLayout())
		return false;

	return true;
}
