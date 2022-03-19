#include "SceneMode.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"

CSceneMode::CSceneMode() :
m_Scene(nullptr),
m_PlayerObject(nullptr),
m_IsEditMode(false)
{
	SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::SetPlayerObject(CGameObject* Obj)
{
	m_PlayerObject = Obj;
}

void CSceneMode::SetTileMapEmptyObject(CGameObject* Obj)
{
	m_TileMapEmptyObject = Obj;
}

void CSceneMode::SetLineContainerObject(CGameObject* Obj)
{
	m_LineContainerObject = Obj;
}

void CSceneMode::SetSceneChangeObject(CGameObject* Obj)
{
	m_SceneChangeObject = Obj;
}

void CSceneMode::Start()
{
	if (m_Scene->GetPlayerObject())
	{
		m_Scene->GetPlayerObject()->Enable(true);
	}
}

bool CSceneMode::Init()
{
	return true;
}

void CSceneMode::Update(float DeltaTime)
{
}

void CSceneMode::PostUpdate(float DeltaTime)
{
}

void CSceneMode::PrepareResources()
{
	// 기본 Material 생성해두기
	m_Scene->GetResource()->CreateMaterial<CMaterial>("Color");

	CSharedPtr<CMaterial> Mtrl = m_Scene->GetResource()->FindMaterial("Color");

	Mtrl->SetShader("ColorMeshShader");

	m_Scene->GetResource()->CreateMaterial<CMaterial>("BaseTexture");

	Mtrl = m_Scene->GetResource()->FindMaterial("BaseTexture");

	Mtrl->SetShader("Mesh2DShader");

	CTexture* Texture = CResourceManager::GetInst()->FindTexture("EngineTexture");

	if (Mtrl->IsTextureEmpty())
	{
		Mtrl->AddTexture(0, static_cast<int>(Buffer_Shader_Type::Pixel), "EngineTexture", Texture);
	}
}
