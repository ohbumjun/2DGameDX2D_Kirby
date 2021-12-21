#include "StaticMeshComponent.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	SetTypeID<CStaticMeshComponent>();
	m_Render = true;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& Mesh) :
	CSceneComponent(Mesh)
{
	m_Mesh = Mesh.m_Mesh;
	if (Mesh.m_Material)
		m_Material = Mesh.m_Material->Clone();
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::SetMesh(const std::string& Name)
{
	m_Mesh = dynamic_cast<CStaticMesh*>(m_Scene->GetResource()->FindMesh(Name));
	// m_Mesh =(CStaticMesh*)m_Scene->GetResource()->FindMesh(Name);
}

void CStaticMeshComponent::SetMesh(CStaticMesh* Mesh)
{
	m_Mesh = Mesh;
}

void CStaticMeshComponent::SetMaterial(const CMaterial* Material)
{
	m_Material = const_cast<CMaterial*>(Material)->Clone();
	m_Material->SetScene(m_Scene);
}

void CStaticMeshComponent::SetBaseColor(const Vector4& Color)
{
	m_Material->SetBaseColor(Color);
}

void CStaticMeshComponent::SetBaseColor(float r, float g, float b, float a)
{
	m_Material->SetBaseColor(r, g, b, a);
}

void CStaticMeshComponent::SetRenderState(const std::string& Name)
{
	m_Material->SetRenderState(Name);
}

void CStaticMeshComponent::SetRenderState(const CRenderState* State)
{
	m_Material->SetRenderState(const_cast<CRenderState*>(State));
}

void CStaticMeshComponent::SetTransparency(bool Enable)
{
	m_Material->SetTransparency(Enable);
}

void CStaticMeshComponent::SetOpacity(float Opacity)
{
	m_Material->SetOpacity(Opacity);
}

void CStaticMeshComponent::AddOpacity(float Opacity)
{
	m_Material->AddOpacity(Opacity);
}

void CStaticMeshComponent::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->AddTexture(Register, ShaderType, Name, Texture);
}

void CStaticMeshComponent::AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
                                      const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CStaticMeshComponent::AddTextureFullPath(int          Register, int ShaderType, const std::string& Name,
                                              const TCHAR* FullPath)
{
	m_Material->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CStaticMeshComponent::AddTexture(int                        Register, int ShaderType, const std::string& Name,
                                      const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CStaticMeshComponent::SetTexture(int       Index, int Register, int ShaderType, const std::string& Name,
                                      CTexture* Texture)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CStaticMeshComponent::SetTexture(int                Index, int Register, int ShaderType, const std::string& Name,
                                      const TCHAR*       FileName,
                                      const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CStaticMeshComponent::SetTextureFullPath(int          Index, int Register, int ShaderType, const std::string& Name,
                                              const TCHAR* FullPath)
{
	m_Material->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CStaticMeshComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
                                      const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

bool CStaticMeshComponent::Init()
{
	SetMaterial(m_Scene->GetResource()->FindMaterial("Color"));
	return true;
}

void CStaticMeshComponent::Start()
{
	CSceneComponent::Start();
}

void CStaticMeshComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CStaticMeshComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CStaticMeshComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CStaticMeshComponent::Render()
{
	CSceneComponent::Render();

	if (!m_Material || !m_Mesh)
		return;

	m_Material->Render();

	m_Mesh->Render();

	m_Material->Reset();
}

void CStaticMeshComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}
