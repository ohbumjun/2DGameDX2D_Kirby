#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

 CTileMapComponent::CTileMapComponent()
 {
	 SetTypeID<CTileMapComponent>();
	 m_Render = true;
 }

 CTileMapComponent::CTileMapComponent(const CTileMapComponent& com)
 {
	 m_BackMesh = com.m_BackMesh;
	 if (com.m_BackMaterial)
		 m_BackMaterial = com.m_BackMaterial->Clone();
 }

 CTileMapComponent::~CTileMapComponent()
{}

 void CTileMapComponent::SetMaterial(CMaterial* Material)
{}

 void CTileMapComponent::SetBackBaseColor(const Vector4& Color)
{}

 void CTileMapComponent::SetBackBaseColor(float r, float g, float b, float a)
{}

 void CTileMapComponent::SetBackRenderState(CRenderState* State)
{}

 void CTileMapComponent::SetBackRenderState(const std::string& Name)
{}

 void CTileMapComponent::SetBackTransparency(bool Enable)
{}

 void CTileMapComponent::SetBackOpacity(float Opacity)
{}

 void CTileMapComponent::AddBackOpacity(float Opacity)
{}

 void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{}

 void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{}

 void CTileMapComponent::AddBackTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{}

 void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{}

 void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{}

 void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{}

 void CTileMapComponent::SetBackTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{}

 void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{}

 void CTileMapComponent::Start()
{
	CSceneComponent::Start();
}

 bool CTileMapComponent::Init()
{
	 if (!CSceneComponent::Init())
		 return false;

	 m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");
	 m_BackMaterial = m_Scene->GetResource()->FindMaterial("BaseTexture");
	 SetMeshSize(1.f, 0.f, 0.f);
	 SetWorldScale((float)m_BackMaterial->GetTexture()->GetWidth(),
		 (float)m_BackMaterial->GetTexture()->GetHeight(), 1.f);
}

 void CTileMapComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

 void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

 void CTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

 void CTileMapComponent::Render()
{
	CSceneComponent::Render();

	if (!m_BackMaterial || !m_BackMesh)
		return;

	m_BackMaterial->Render();

	m_BackMesh->Render();

	m_BackMaterial->Reset();
}

 void CTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

 CTileMapComponent* CTileMapComponent::Clone()
{
	 return new CTileMapComponent(*this);
}

 void CTileMapComponent::Save(FILE* File)
{
	 int MeshNameLength = (int)m_BackMesh->GetName().length();
	 fwrite(&MeshNameLength, sizeof(int), 1, File);
	 fwrite(m_BackMesh->GetName().c_str(), sizeof(char), MeshNameLength, File);

	 m_BackMaterial->Save(File);

	CSceneComponent::Save(File);
}

 void CTileMapComponent::Load(FILE* File)
{
	 int NameLength = 0;
	 fread(&NameLength, sizeof(int), 1, File);

	 char MeshName[MAX_PATH] = {};
	 fread(MeshName, sizeof(char), NameLength, File);
	 m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	 m_BackMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
	 m_BackMaterial->Load(File);

	CSceneComponent::Load(File);
}