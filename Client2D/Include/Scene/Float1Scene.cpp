#include "Float1Scene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "../Object/LineContainer.h"
#include "GameObject/Line.h"
#include "../Object/EffectSceneChangeAlpha.h"

CFloat1Scene::CFloat1Scene()
{}

CFloat1Scene::~CFloat1Scene()
{}

void CFloat1Scene::Start()
{
	CSceneMode::Start();

	// Scene Change ȿ��
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(true);


	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());


	if (Player2D)
	{
		SetPlayerObject(Player2D);
		Player2D->SetWorldPos(200.f, 900.f, 0.f);
	}

	if (CSceneManager::GetStaticPlayerInfo())
	{
		if (m_Scene->GetSceneChangeObject())
		{
			Vector3 PlayerSpawnBasePos = m_Scene->GetSceneChangeObject()->GetWorldPos();

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, PlayerSpawnBasePos.z);
		}
	}
}

bool CFloat1Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CFloat1Scene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CFloat1Scene::CreateAnimationSequence()
{}

void CFloat1Scene::CreateSound()
{}

void CFloat1Scene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CFloat1Scene::PrepareResources()
{
	// Material�� �����Ѵ�.

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.2f);

	Sleep(500);

	CSceneMode::PrepareResources();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.6f);

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	Sleep(500);

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.85f);

	Sleep(500); ////

	CreateSound();
}
