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
	// 현재 프로젝트 상에서는, ResourceManager 로부터, Animation 을 Load 한 다음에 불러오는 것이다.
	// 실제 지워주는 것도 ResourceManager 측에서 처리하므로
	// 각각의 SpriteComponent 마다 해당 처리르 해줄 필요가 없다.....?
	// 아니다. 각각의 SceneResource 에서만 관리를 해주자... 굳이 ResourceManager 까지 ?
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

	CAnimationSequence2DInstance* CloneInstance = Instance->Clone();

	CloneInstance->SetOwner(this);
	CloneInstance->SetScene(m_Scene);

	if (!CloneInstance->Init()) // 상수 버퍼 세팅 
	{
		SAFE_DELETE(CloneInstance);
		return;
	}

	SAFE_DELETE(m_Animation);

	m_Animation = CloneInstance;
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
	// Mesh 세팅 
	m_Mesh = dynamic_cast<CSpriteMesh*>(m_Scene->GetResource()->FindMesh("SpriteMesh"));
	SetMeshSize(1.f, 1.f, 0.f);

	// Material Setting 
	SetMaterial(m_Scene->GetResource()->FindMaterial("BaseTexture"));

	// "BaseTexutre"라는 이름의 Material이 지닌 기본 Default Texture로 SpriteComponent의 World Scale 세팅
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

	if (!m_Material || !m_Mesh)
		return;

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

	// Mesh의 경우, 이름만 저장해두고, ResourceManager 로부터 불러오는 코드를 작성할 것이다.
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

	// SceneComponent Save  에서는 자식 SceneCompoent 목록들을 다 저장하는 코드를 거쳐서 들어간다
	// 되도록이면, 자기의 고유의 정보를 먼저 저장하고
	// 그 다음 자식 목록 정보를 저장하기 위해, 의도적으로 순서를 이렇게 짠다.
	CSceneComponent::Save(pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
	// Mesh의 경우, 이름만 저장해두고, ResourceManager 로부터 불러오는 코드를 작성할 것이다.
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

	// SceneComponent Save  에서는 자식 SceneCompoent 목록들을 다 저장하는 코드를 거쳐서 들어간다
	// 되도록이면, 자기의 고유의 정보를 먼저 저장하고
	// 그 다음 자식 목록 정보를 저장하기 위해, 의도적으로 순서를 이렇게 짠다.
	CSceneComponent::Load(pFile);
}
