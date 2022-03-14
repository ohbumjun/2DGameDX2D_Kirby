#include "Green2Scene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "../Object/LineContainer.h"

CGreen2Scene::CGreen2Scene()
{}

CGreen2Scene::~CGreen2Scene()
{}

void CGreen2Scene::Start()
{
	CSceneMode::Start();

	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);
}

bool CGreen2Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CGreen2Scene::CreateMaterial()
{}

void CGreen2Scene::CreateAnimationSequence()
{}

void CGreen2Scene::CreateSound()
{}

void CGreen2Scene::CreateParticle()
{}

void CGreen2Scene::PrepareResources()
{
	CSceneMode::PrepareResources();
}
