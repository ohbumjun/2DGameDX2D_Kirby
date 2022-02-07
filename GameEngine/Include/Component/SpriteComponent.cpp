#include "SpriteComponent.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"

CSpriteComponent::CSpriteComponent() :
	m_Animation(nullptr)
{
	SetTypeID<CSpriteComponent>();
	m_Render = true;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com) :
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

	if (com.m_Animation)
		m_Animation = com.m_Animation->Clone();

	if (com.m_Material)
		m_Material = com.m_Material->Clone();
}

CSpriteComponent::~CSpriteComponent()
{
	SAFE_DELETE(m_Animation);
}

void CSpriteComponent::SetMaterial(CMaterial* Material)
{
	m_Material = Material->Clone();

	m_Material->SetScene(m_Scene);
}

void CSpriteComponent::SetAnimationInstance(class CAnimationSequence2DInstance* Instance)
{
	if (!Instance)
		return;

	Instance->SetOwner(this);
	Instance->SetScene(m_Scene);
	if (!Instance->Init()) // ��� ���� ���� 
	{
		SAFE_DELETE(Instance);
		return;
	}

	SAFE_DELETE(m_Animation);
	m_Animation = Instance;
}

void CSpriteComponent::SetBaseColor(const Vector4& Color)
{
	m_Material->SetBaseColor(Color);
}

void CSpriteComponent::SetBaseColor(float r, float g, float b, float a)
{
	m_Material->SetBaseColor(r, g, b, a);
}

void CSpriteComponent::SetRenderState(CRenderState* State)
{
	m_Material->SetRenderState(State);
}

void CSpriteComponent::SetRenderState(const std::string& Name)
{
	m_Material->SetRenderState(Name);
}

void CSpriteComponent::SetTransparency(bool Enable)
{
	m_Material->SetTransparency(Enable);
}

void CSpriteComponent::SetOpacity(float Opacity)
{
	m_Material->SetOpacity(Opacity);
}

void CSpriteComponent::AddOpacity(float Opacity)
{
	m_Material->AddOpacity(Opacity);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->AddTexture(Register, ShaderType, Name, Texture);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
                                  const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CSpriteComponent::AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
	m_Material->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CSpriteComponent::AddTexture(int                        Register, int ShaderType, const std::string& Name,
                                  const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CSpriteComponent::SetTexture(int Index, class CTexture* Texture)
{
	m_Material->SetTexture(Index, Texture);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, Texture);
	// SetWorldScale((float)m_Material->GetTextureWidth(), (float)m_Material->GetTextureHeight(), 1.f);
}

void CSpriteComponent::SetTexture(int                Index, int Register, int ShaderType, const std::string& Name,
                                  const TCHAR*       FileName,
                                  const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CSpriteComponent::SetTextureFullPath(int          Index, int Register, int ShaderType, const std::string& Name,
                                          const TCHAR* FullPath)
{
	m_Material->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
                                  const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CSpriteComponent::SetTextureWorldScale(int Index)
{
	if (!m_Material->GetTexture(Index))
		return;
	SetWorldScale((float)m_Material->GetTextureWidth(Index), (float)m_Material->GetTextureHeight(Index), 1.f);
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();

	if (m_Animation)
		m_Animation->Start();
}

bool CSpriteComponent::Init()
{
	// Mesh ���� 
	m_Mesh = dynamic_cast<CSpriteMesh*>(m_Scene->GetResource()->FindMesh("SpriteMesh"));
	SetMeshSize(1.f, 1.f, 0.f);

	// Material Setting 
	SetMaterial(m_Scene->GetResource()->FindMaterial("BaseTexture"));

	// "BaseTexutre"��� �̸��� Material�� ���� �⺻ Default Texture�� SpriteComponent�� World Scale ����
	float Width = (float)m_Material->GetTextureWidth();
	float Height = (float)m_Material->GetTextureHeight();
	SetWorldScale(Width, Height, 1.f);

	return true;
}

void CSpriteComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CSpriteComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CSpriteComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CSpriteComponent::Render()
{
	CSceneComponent::Render();

	if (m_Animation)
	{
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(m_Animation->GetAnimationCount() > 0);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();

		m_Animation->SetShader();
	}

	m_Material->Render();

	m_Mesh->Render();

	m_Material->Reset();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::Save(FILE* pFile)
{
	/*
	CSharedPtr<CSpriteMesh>             m_Mesh;
	CSharedPtr<CMaterial>               m_Material;
	class CAnimationSequence2DInstance* m_Animation;
	*/

	// Mesh�� ���, �̸��� �����صΰ�, ResourceManager �κ��� �ҷ����� �ڵ带 �ۼ��� ���̴�.
	std::string Name = m_Mesh->GetName();
	int Length = (int)Name.length();
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(Name.c_str(), sizeof(char), Length, pFile); 

	// Mateiral
	m_Material->Save(pFile);

	bool Animation = false;
	if (m_Animation)
		Animation = true;
	fwrite(&m_Animation, sizeof(bool), 1, pFile);

	if (Animation)
	{
		size_t TypeID = m_Animation->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, pFile);
		m_Animation->Save(pFile);
	}

	// SceneComponent Save  ������ �ڽ� SceneCompoent ��ϵ��� �� �����ϴ� �ڵ带 ���ļ� ����
	// �ǵ����̸�, �ڱ��� ������ ������ ���� �����ϰ�
	// �� ���� �ڽ� ��� ������ �����ϱ� ����, �ǵ������� ������ �̷��� §��.
	CSceneComponent::Save(pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
	// Mesh�� ���, �̸��� �����صΰ�, ResourceManager �κ��� �ҷ����� �ڵ带 �ۼ��� ���̴�.
	int Length = -1;
	fread(&Length, sizeof(int), 1, pFile);

	char Name[MAX_PATH] = {};
	fread(&Name, sizeof(char), Length, pFile);
	m_Mesh = (CSpriteMesh*)CResourceManager::GetInst()->FindMesh(Name);

	// Mateiral
	m_Material = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();
	m_Material->Load(pFile);

	bool Animation = false;
	fread(&Animation, sizeof(bool), 1, pFile);

	if (Animation)
	{
		size_t TypeID = -1;
		fread(&TypeID, sizeof(size_t), 1, pFile);
		CSceneManager::GetInst()->CallCreateAnimInstanceFunc(this, TypeID);
		m_Animation->Load(pFile);
	}

	// SceneComponent Save  ������ �ڽ� SceneCompoent ��ϵ��� �� �����ϴ� �ڵ带 ���ļ� ����
	// �ǵ����̸�, �ڱ��� ������ ������ ���� �����ϰ�
	// �� ���� �ڽ� ��� ������ �����ϱ� ����, �ǵ������� ������ �̷��� §��.
	CSceneComponent::Load(pFile);
}
