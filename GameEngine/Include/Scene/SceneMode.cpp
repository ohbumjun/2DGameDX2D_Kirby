#include "SceneMode.h"
#include "../GameObject/GameObject.h"

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

void CSceneMode::Start()
{
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
{}
