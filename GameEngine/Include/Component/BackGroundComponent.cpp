#include "BackGroundComponent.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "../Scene/NavigationManager.h"
#include "../Scene/SceneCollision.h"

class CStaticMesh;

CBackGroundComponent::CBackGroundComponent()
{
	SetTypeID<CBackGroundComponent>();
	m_Render = true;

	m_LayerName = "Back";

	m_DeltaTime = 0.f;

	m_EditMode = false;
	
}

CBackGroundComponent::CBackGroundComponent(const CBackGroundComponent& Component)
{
	m_BackGroundMesh = Component.m_BackGroundMesh;

	if (Component.m_BackGroundMaterial)
		m_BackGroundMaterial = Component.m_BackGroundMaterial->Clone();

	m_EditMode = Component.m_EditMode;
}

CBackGroundComponent::~CBackGroundComponent()
{
}

void CBackGroundComponent::SetBackGroundMaterial(CMaterial* Material)
{
	m_BackGroundMaterial = Material->Clone();

	m_BackGroundMaterial->SetScene(m_Scene);

	if (!m_BackGroundMaterial->EmptyTexture())
	{
		SetWorldScale((float)m_BackGroundMaterial->GetTextureWidth(),
			(float)m_BackGroundMaterial->GetTextureHeight(), 1.f);
	}
}

void CBackGroundComponent::SetBackGroundBaseColor(const Vector4& Color)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetBaseColor(Color);
}

void CBackGroundComponent::SetBackGroundBaseColor(float r, float g, float b, float a)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetBaseColor(r, g, b, a);
}

void CBackGroundComponent::SetBackGroundRenderState(CRenderState* State)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetRenderState(State);
}

void CBackGroundComponent::SetBackGroundRenderState(const std::string& Name)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetRenderState(Name);
}

void CBackGroundComponent::SetBackGroundTransparency(bool Enable)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTransparency(Enable);
}

void CBackGroundComponent::SetBackGroundOpacity(float Opacity)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetOpacity(Opacity);
}

void CBackGroundComponent::AddBackGroundOpacity(float Opacity)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddOpacity(Opacity);
}

void CBackGroundComponent::AddBackGroundTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTexture(Register, ShaderType, Name, Texture);
}

void CBackGroundComponent::AddBackGroundTexture(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CBackGroundComponent::AddBackGroundTextureFullPath(int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CBackGroundComponent::AddBackGroundTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CBackGroundComponent::SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name,
	CTexture* Texture)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CBackGroundComponent::SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CBackGroundComponent::SetBackGroundTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CBackGroundComponent::SetBackGroundTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackGroundMaterial)
		return;

	m_BackGroundMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CBackGroundComponent::Start()
{
	CSceneComponent::Start();
}

bool CBackGroundComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	// Back Mesh
	m_BackGroundMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");

	SetMeshSize(1.f, 1.f, 0.f);

	// m_BackGroundMaterial Material
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BackGroundMaterial");

	m_BackGroundMaterial = m_Scene->GetResource()->FindMaterial("BackGroundMaterial");

	m_BackGroundMaterial->SetShader("Mesh2DShader");

	SetWorldScale(100.f, 100.f, 1.f);

	return true;
}

void CBackGroundComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CBackGroundComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	m_DeltaTime = DeltaTime;
}

void CBackGroundComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CBackGroundComponent::Render()
{
	CSceneComponent::Render();

	Vector3 OriginalPos = GetWorldPos();

	// 배경 먼저 띄우고
	if (m_BackGroundMaterial)
	{
		// 1st
		m_BackGroundMaterial->Render();

		m_BackGroundMesh->Render();

		m_BackGroundMaterial->Reset();

	}
}

void CBackGroundComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CBackGroundComponent* CBackGroundComponent::Clone()
{
	return new CBackGroundComponent(*this);
}

void CBackGroundComponent::Save(FILE* File)
{
	// Back Mesh
	std::string BackMeshName = m_BackGroundMesh->GetName();

	int BackMeshNameLength = (int)BackMeshName.length();

	fwrite(&BackMeshNameLength, sizeof(int), 1, File);
	fwrite(BackMeshName.c_str(), sizeof(char), BackMeshNameLength, File);

	// Back Material
	bool BackMaterialEnable = false;

	if (m_BackGroundMaterial)
		BackMaterialEnable = true;

	fwrite(&BackMaterialEnable, sizeof(bool), 1, File);

	if (BackMaterialEnable)
		m_BackGroundMaterial->Save(File);

	CSceneComponent::Save(File);
}

void CBackGroundComponent::Load(FILE* File)
{
	// Back Mesh
	char	BackMeshName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(BackMeshName, sizeof(char), Length, File);

	m_BackGroundMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(BackMeshName);

	SetMeshSize(1.f, 1.f, 0.f);

	// Back Material
	bool BackMaterialEnable = false;

	fread(&BackMaterialEnable, sizeof(bool), 1, File);

	if (BackMaterialEnable)
	{
		m_BackGroundMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
		m_BackGroundMaterial->Load(File);
	}

	m_EditMode = false;

	CSceneComponent::Load(File);
}
