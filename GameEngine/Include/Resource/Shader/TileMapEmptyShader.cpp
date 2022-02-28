#include "TileMapEmptyShader.h"

CTileMapEmptyShader::CTileMapEmptyShader()
{}

CTileMapEmptyShader::CTileMapEmptyShader(const CTileMapEmptyShader& Shader)
{}

CTileMapEmptyShader::~CTileMapEmptyShader()
{}

bool CTileMapEmptyShader::Init()
{
	if (!LoadVertexShader("TileMapEmptyVS", TEXT("TileMapEmpty.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("TileMapEmptyPS", TEXT("TileMapEmpty.fx"), SHADER_PATH))
		return false;

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	return true;
}
