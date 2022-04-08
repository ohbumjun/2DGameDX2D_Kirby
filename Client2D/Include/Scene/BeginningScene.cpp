#include "BeginningScene.h"
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
#include "../UI/IntroWidget.h"
#include "../Object/LineContainer.h"
#include "../Object/EffectSceneChangeAlpha.h"

CBeginningScene::CBeginningScene()
{
	SetTypeID<CBeginningScene>();
}

CBeginningScene::~CBeginningScene()
{}

void CBeginningScene::Start()
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

	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);

	if (CSceneManager::GetStaticPlayerInfo())
	{
		if (m_Scene->GetSceneChangeObject())
		{
			Vector3 PlayerSpawnBasePos = m_Scene->GetSceneChangeObject()->GetWorldPos();

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, PlayerSpawnBasePos.z);
		}
	}

	// m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroWidget>("IntroWidget");
}

bool CBeginningScene::Init()
{
	if (!CSceneMode::Init())
		return false;


	return true;
}

void CBeginningScene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CBeginningScene::CreateAnimationSequence()
{}

void CBeginningScene::CreateSound()
{
	CSceneMode::CreateSound();
	m_Scene->GetResource()->LoadSound("BGM", "BG_Green",
		true, TEXT("BG_Green.mp3"));
	m_Scene->GetResource()->SoundPlay("BG_Green");
}

void CBeginningScene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CBeginningScene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
