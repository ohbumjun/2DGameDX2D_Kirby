#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "ConstantBuffer.h"
#include "Mesh2DShader.h"
#include "PosMeshShader.h"
#include "ColliderShader.h"
#include "ColliderPixelShader.h"
#include "WidgetConstantBuffer.h"

CShaderManager::CShaderManager()
{
}

CShaderManager::~CShaderManager()
{
}

bool CShaderManager::Init()
{
	if (!CreateShader<CColorMeshShader>("ColorMeshShader"))
		return false;

	if (!CreateShader<CMesh2DShader>("Mesh2DShader"))
		return false;

	if (!CreateShader<CPosMeshShader>("PosMeshShader"))
		return false;

	if (!CreateShader<CColliderShader>("ColliderShader"))
		return false;

	// ColliderPixelShader
	if (!CreateShader<CColliderPixelShader>("ColliderPixelShader"))
		return false;

	if (!CreateShader<CWidgetConstantBuffer>("WidgetShader"))
		return false;

	// =================== 상수버퍼 ===================
	CreateConstantBuffer("TransformCBuffer", sizeof(TransformCBuffer), 0,
	                     static_cast<int>(ConstantBuffer_Shader_Type::Graphic));

	CreateConstantBuffer("MaterialCBuffer", sizeof(MaterialCBuffer), 1,
	                     static_cast<int>(ConstantBuffer_Shader_Type::Graphic));

	CreateConstantBuffer("Standard2DCBuffer", sizeof(Standard2DCBuffer), 2,
	                     static_cast<int>(ConstantBuffer_Shader_Type::Graphic));

	CreateConstantBuffer("Animation2DCBuffer", sizeof(Animation2DCBuffer), 10,
	                     static_cast<int>(ConstantBuffer_Shader_Type::Graphic));

	CreateConstantBuffer("ColliderCBuffer", sizeof(ColliderCBuffer), 11,
		static_cast<int>(ConstantBuffer_Shader_Type::Graphic));

	CreateConstantBuffer("WidgetConstantBuffer", sizeof(WidgetCBuffer), 11,
		(int)(ConstantBuffer_Shader_Type::Graphic));

	return true;
}

CShader* CShaderManager::FindShader(const std::string& Name)
{
	auto iter = m_mapShader.find(Name);

	if (iter == m_mapShader.end())
		return nullptr;

	return iter->second;
}

void CShaderManager::ReleaseShader(const std::string& Name)
{
	auto iter = m_mapShader.find(Name);

	if (iter != m_mapShader.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapShader.erase(iter);
	}
}

bool CShaderManager::CreateConstantBuffer(const std::string& Name, int Size, int Register,
                                          int                ConstantBufferShaderType)
{
	CConstantBuffer* Buffer = FindConstantBuffer(Name);

	if (Buffer)
		return true;

	Buffer = new CConstantBuffer;

	Buffer->SetName(Name);

	if (!Buffer->Init(Size, Register, ConstantBufferShaderType))
	{
		SAFE_RELEASE(Buffer);
		return false;
	}

	m_mapConstantBuffer.insert(std::make_pair(Name, Buffer));

	return true;
}

CConstantBuffer* CShaderManager::FindConstantBuffer(const std::string& Name)
{
	auto iter = m_mapConstantBuffer.find(Name);

	if (iter == m_mapConstantBuffer.end())
		return nullptr;

	return iter->second;
}
