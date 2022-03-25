#include "Green2Scene.h"

#include <Scene/CameraManager.h>

#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "../Object/LineContainer.h"
#include "../Object/EffectSceneChangeAlpha.h"

CGreen2Scene::CGreen2Scene()
{}

CGreen2Scene::~CGreen2Scene()
{}

void CGreen2Scene::Start()
{
	CSceneMode::Start();

	// Scene Change È¿°ú
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(true);

	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	m_Scene->GetCameraManager()->GetCurrentCamera()->SetWorldPos(Player2D->GetWorldPos());
	
	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);

	if (CSceneManager::GetStaticPlayerInfo())
	{
		if (m_Scene->GetSceneChangeObject())
		{
			Vector3 PlayerSpawnBasePos = m_Scene->GetSceneChangeObject()->GetWorldPos();

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, 0.f);
		}
	}
}

bool CGreen2Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CGreen2Scene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CGreen2Scene::CreateAnimationSequence()
{}

void CGreen2Scene::CreateSound()
{}

void CGreen2Scene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CGreen2Scene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
