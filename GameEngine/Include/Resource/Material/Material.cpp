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

CMaterial::CMaterial(const CMaterial& Material) :CRef(Material)
{
	*this = Material;

	m_RefCount = 0;

	m_Scene = nullptr;

	m_CBuffer = Material.m_CBuffer->Clone();

	m_RenderCallbackList.clear();
}

CMaterial::~CMaterial()
{
	auto iter = m_RenderCallbackList.begin();
	auto iterEnd = m_RenderCallbackList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_CBuffer);

	m_RenderCallbackList.clear();
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
	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return;

	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;
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
	m_TextureInfo[Index].ShaderType = ShaderType;
	m_TextureInfo[Index].Texture    = Texture;
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

	auto iter = m_RenderCallbackList.begin();
	auto iterEnd = m_RenderCallbackList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Callback();
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
	// Shader의 경우, Name을 가져와서 세팅할 것이다.
	CRef::Save(pFile);
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
	CRef::Load(pFile);

	// Shader의 경우, Name을 가져와서 세팅할 것이다.
	char ShaderName[MAX_PATH] = {};
	int Length = -1;
	fread(&Length, sizeof(int), 1, pFile);
	fread(ShaderName, sizeof(char), Length, pFile);
	m_Shader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(ShaderName);

	fread(&m_BaseColor, sizeof(Vector4), 1, pFile);
	fread(&m_Opacity, sizeof(float), 1, pFile);

	bool StateEnable = false;
	char RenderStateName[MAX_PATH] = {};
	for (int i = 0; i < (int)RenderState_Type::Max; i++)
	{
		fread(&StateEnable, sizeof(bool), 1, pFile);
		if (StateEnable)
		{
			// Render State 또한, Resource로부터 가져올 예정이므로 , Name 만 저장한다
			fread(&Length, sizeof(int), 1, pFile);
			fread(RenderStateName, sizeof(char), Length, pFile);
			m_RenderStateArray[i] = CRenderManager::GetInst()->FindRenderState(RenderStateName);
		}
	}

	int TextureCount = -1;
	fread(&TextureCount, sizeof(int), 1, pFile);
	char TextureName[MAX_PATH] = {};

	for (int i = 0; i < TextureCount; i++)
	{
		m_TextureInfo.push_back(MaterialTextureInfo());

		fread(&Length, sizeof(int), 1, pFile);
		fread(TextureName, sizeof(char), Length, pFile);
		m_TextureInfo[i].Name = TextureName;

		fread(&m_TextureInfo[i].SamplerType, sizeof(Sampler_Type), 1, pFile);
		fread(&m_TextureInfo[i].Register, sizeof(int), 1, pFile);
		fread(&m_TextureInfo[i].ShaderType, sizeof(int), 1, pFile);

		char TexName[MAX_PATH] = {};
		int Length = -1;
		fread(&Length, sizeof(int), 1, pFile);
		fread(TexName, sizeof(char), Length, pFile);

		Image_Type ImageType;
		fread(&ImageType, sizeof(Image_Type), 1, pFile);

		int InfoCount = -1;
		fread(&InfoCount, sizeof(int), 1, pFile);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string PathName;

		TCHAR FullPath[MAX_PATH] = {};
		TCHAR FileName[MAX_PATH] = {};
		char Path[MAX_PATH] = {};

		for (int i = 0; i < InfoCount; i++)
		{
			
			Length = -1;
			fread(&Length, sizeof(int), 1, pFile);
			fread(FullPath, sizeof(TCHAR), Length, pFile);
			vecFullPath.push_back(FullPath);

			fread(&Length, sizeof(int), 1, pFile);
			fread(Path, sizeof(char), Length, pFile);
			PathName = Path;

			fread(&Length, sizeof(int), 1, pFile);
			fread(FileName, sizeof(TCHAR), Length, pFile);
			vecFileName.push_back(FileName);
		}

		switch(ImageType)
		{
		case Image_Type::Atlas :
			{
				if (vecFileName.size() == 1)
				{
					if (m_Scene)
					{
						// m_Scene->GetResource()->LoadTexture(TexName, vecFileName[0].c_str(), PathName);
						m_Scene->GetResource()->LoadTextureFullPath(TexName, vecFullPath[0].c_str());
					}
					else
					{
						// CResourceManager::GetInst()->LoadTexture(TexName, vecFileName[0].c_str(), PathName);
						CResourceManager::GetInst()->LoadTextureFullPath(TexName, vecFullPath[0].c_str());
					}
				}
				else
				{
				}
			}
			break;
		case Image_Type::Frame:
			{
				if (vecFileName.size() == 1)
				{

				}
				else
				{

				}
			}
			break;
		case Image_Type::Array:
			{
				if (vecFileName.size() == 1)
				{

				}
				else
				{

				}
			}
			break;
		}

		if (m_Scene)
			m_TextureInfo[i].Texture = m_Scene->GetResource()->FindTexture(TexName);
		else 
			m_TextureInfo[i].Texture = CResourceManager::GetInst()->FindTexture(TexName);
	}
}
