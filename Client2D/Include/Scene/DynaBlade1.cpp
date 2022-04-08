#include "DynaBlade1.h"
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
#include "../UI/PlayerHUD.h"
#include "../Object/LineContainer.h"
#include "GameObject/Line.h"
#include "../Object/EffectSceneChangeAlpha.h"

CDynaBlade1::CDynaBlade1()
{}

CDynaBlade1::~CDynaBlade1()
{}

void CDynaBlade1::Start()
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

	// Main Widget
	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CPlayerHUD>("MainWidget");

	m_Scene->SetPlayerHUD(m_MainWidget);

	if (CSceneManager::GetStaticPlayerInfo())
	{
		CPlayer2D* Player = dynamic_cast<CPlayer2D*>(CSceneManager::GetStaticPlayerInfo());

		// 1) PlayerHUD �� Player HP, MP �����ϱ�
		CPlayerHUD* PlayerHUD = (CPlayerHUD*)m_MainWidget;

		PlayerHUD->GetHPProgressBar()->SetPercent(Player->GetHP() / Player->GetHPMax());
		PlayerHUD->GetMPProgressBar()->SetPercent(Player->GetMP() / Player->GetMPMax());

		if (m_Scene->GetSceneChangeObject())
		{
			Vector3 PlayerSpawnBasePos = m_Scene->GetSceneChangeObject()->GetWorldPos();

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, PlayerSpawnBasePos.z);
		}
	}
}

bool CDynaBlade1::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CDynaBlade1::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CDynaBlade1::CreateAnimationSequence()
{}

void CDynaBlade1::CreateSound()
{
	CSceneMode::CreateSound();
	m_Scene->GetResource()->LoadSound("BGM", "BG_ConCave1",
		true, TEXT("BG_ConCave1.mp3"));
	m_Scene->GetResource()->SoundPlay("BG_ConCave1");
}

void CDynaBlade1::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CDynaBlade1::PrepareResources()
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
