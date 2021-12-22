#include "Material.h"
#include "../ResourceManager.h"
#include "../../Render/RenderManager.h"
#include "../../Render/RenderState.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/MaterialConstantBuffer.h"

CMaterial::CMaterial() :
	m_Scene(nullptr),
	m_BaseColor(Vector4::White),
	m_Opacity(1.f),
	m_CBuffer(nullptr),
	m_RenderStateArray{}
{
	SetTypeID<CMaterial>();
}

CMaterial::CMaterial(const CMaterial& Material)
{
	*this = Material;

	m_RefCount = 0;

	m_Scene = nullptr;
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetRenderState(CRenderState* State)
{
	m_RenderStateArray[static_cast<int>(RenderState_Type::Blend)] = State;
}

void CMaterial::SetRenderState(const std::string& Name)
{
	m_RenderStateArray[static_cast<int>(RenderState_Type::Blend)] = CRenderManager::GetInst()->FindRenderState(Name);
}

void CMaterial::SetTransparency(bool Enable)
{
	if (Enable)
		m_RenderStateArray[static_cast<int>(RenderState_Type::Blend)] = CRenderManager::GetInst()->
			FindRenderState("AlphaBlend");
}

void CMaterial::SetOpacity(float Opacity)
{
	m_Opacity = Opacity;

	if (m_Opacity < 0.f)
		m_Opacity = 0.f;

	else if (m_Opacity > 1.f)
		m_Opacity = 1.f;
}

void CMaterial::AddOpacity(float Opacity)
{
	m_Opacity += Opacity;

	if (m_Opacity < 0.f)
		m_Opacity = 0.f;

	else if (m_Opacity > 1.f)
		m_Opacity = 1.f;
}

void CMaterial::SetBaseColor(const Vector4& Color)
{
	m_BaseColor = Color;
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);
}

void CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_TextureInfo.push_back(MaterialTextureInfo());

	int Index = static_cast<int>(m_TextureInfo.size()) - 1;

	m_TextureInfo[Index].Register   = Register;
	m_TextureInfo[Index].Name       = Name;
	m_TextureInfo[Index].Texture    = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::AddTexture(int                Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
                           const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	else
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	m_TextureInfo.push_back(MaterialTextureInfo());

	int Index = static_cast<int>(m_TextureInfo.size()) - 1;

	m_TextureInfo[Index].Register   = Register;
	m_TextureInfo[Index].Name       = Name;
	m_TextureInfo[Index].Texture    = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;*/
}

void CMaterial::AddTexture(int                        Register, int ShaderType, const std::string& Name,
                           const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;*/
}

void CMaterial::SetTexture(int Index, CTexture* Texture)
{
	m_TextureInfo[Index].Texture = Texture;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_TextureInfo[Index].Register   = Register;
	m_TextureInfo[Index].Name       = Name;
	m_TextureInfo[Index].Texture    = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
                           const std::string& PathName)
{
	CTexture* Texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	else
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	m_TextureInfo[Index].Register   = Register;
	m_TextureInfo[Index].Name       = Name;
	m_TextureInfo[Index].Texture    = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTextureFullPath(int                Index, int         Register, int ShaderType,
                                   const std::string& Name, const TCHAR* FullPath)
{
	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return;

	m_TextureInfo[Index].Register   = Register;
	m_TextureInfo[Index].Name       = Name;
	m_TextureInfo[Index].Texture    = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTexture(int                        Index, int Register, int ShaderType, const std::string& Name,
                           const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;*/
}

void CMaterial::SetShader(const std::string& Name)
{
	m_Shader = static_cast<CGraphicShader*>(CResourceManager::GetInst()->FindShader(Name));
}

void CMaterial::Render()
{
	if (m_Shader)
		m_Shader->SetShader();

	if (m_CBuffer)
	{
		m_CBuffer->SetBaseColor(m_BaseColor);
		m_CBuffer->SetOpacity(m_Opacity);

		m_CBuffer->UpdateCBuffer();
	}

	for (int i = 0; i < static_cast<int>(RenderState_Type::Max); ++i)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->SetState();
	}

	size_t Size = m_TextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_TextureInfo[i].Texture->SetShader(m_TextureInfo[i].Register, m_TextureInfo[i].ShaderType, 0);
	}
}

void CMaterial::Reset()
{
	size_t Size = m_TextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_TextureInfo[i].Texture->ResetShader(m_TextureInfo[i].Register, m_TextureInfo[i].ShaderType, 0);
	}

	for (int i = 0; i < static_cast<int>(RenderState_Type::Max); ++i)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->ResetState();
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::Save(FILE* pFile)
{
	CRef::Save(pFile);

	// Shader의 경우, Name을 가져와서 세팅할 것이다. 
	std::string ShaderName = m_Shader->GetName();
	int Length = (int)ShaderName.length();
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(ShaderName.c_str(), sizeof(char), Length, pFile);

	fwrite(&m_BaseColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_Opacity, sizeof(float), 1, pFile);

	for (int i = 0; i < (int)RenderState_Type::Max; i++)
	{
		bool StateEnable = false;
		if (m_RenderStateArray[i])
			StateEnable = true;
		fwrite(&StateEnable, sizeof(bool), 1, pFile);

		if (StateEnable)
		{
			// Render State 또한, Resource로부터 가져올 예정이므로 , Name 만 저장한다
			Length = (int)m_RenderStateArray[i]->GetName().length();
			fwrite(&Length, sizeof(int), 1, pFile);
			fwrite(m_RenderStateArray[i]->GetName().c_str(), sizeof(char), Length, pFile);
		}
	}

	int TextureCount = (int)m_TextureInfo.size();
	fwrite(&TextureCount, sizeof(int), 1, pFile);
	for (int i = 0; i < TextureCount; i++)
	{
		Length = (int)m_TextureInfo[i].Name.length();
		fwrite(&Length, sizeof(int), 1, pFile);
		fwrite(m_TextureInfo[i].Name.c_str(), sizeof(char), Length, pFile);

		fwrite(&m_TextureInfo[i].SamplerType, sizeof(Sampler_Type), 1, pFile);
		fwrite(&m_TextureInfo[i].Register, sizeof(int), 1, pFile);
		fwrite(&m_TextureInfo[i].ShaderType, sizeof(int), 1, pFile);

		m_TextureInfo[i].Texture->Save(pFile);
	}
}

void CMaterial::Load(FILE* pFile)
{
}
