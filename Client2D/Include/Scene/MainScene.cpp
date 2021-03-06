#include "MainScene.h"
#include "../UI/MainWidget.h"
#include "../UI/PlayerHUD.h"
#include "Resource/Particle/Particle.h"
// Scene
#include <Scene/SceneManager.h>
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/ViewPort.h"
// Object
#include <Scene/CameraManager.h>
#include "../Object/Player2D.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/PixelTest.h"
#include "../Object/BubbleParticle.h"
#include "../Object/SpecialChangeParticle.h"
#include "../Object/YellowBird.h"
#include "../Object/BeamMonster.h"
#include "../Object/PurpleBeatle.h"
#include "../Object/NormalBear.h"
#include "../Object/BackGround.h"
#include "../Object/LineContainer.h"
#include "../Object/MoonAboveParticle.h"
#include "../Object/EffectChangeToGreen2.h"
#include "../Object/EffectSceneChangeAlpha.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

void CMainScene::Start()
{
	CSceneMode::Start();

	// Scene Change ȿ??
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(true);

	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	// Player
	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	Player2D->SetWorldPos(100.f, 500.f, 0.f);

	// Line
	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);

	// Main Widget
	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CPlayerHUD>("MainWidget");

	m_Scene->SetPlayerHUD(m_MainWidget);

	// Static Player
	if (CSceneManager::GetStaticPlayerInfo())
	{
		CPlayer2D* Player = dynamic_cast<CPlayer2D*>(CSceneManager::GetStaticPlayerInfo());

		// 1) PlayerHUD ?? Player HP, MP ?????ϱ?
		CPlayerHUD* PlayerHUD = (CPlayerHUD*)m_MainWidget;

		PlayerHUD->GetHPProgressBar()->SetPercent(Player->GetHP() / Player->GetHPMax());
		PlayerHUD->GetMPProgressBar()->SetPercent(Player->GetMP() / Player->GetMPMax());

		// 2) Door ???? Player ??ġ ??Ű?? 
		if (m_Scene->GetSceneChangeObject() && Player->IsBackToSceneChangeDoorPos())
		{
			Vector3 PlayerSpawnBasePos = m_Scene->GetSceneChangeObject()->GetWorldPos();

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, 0.f);

			Player->SetIsBackToSceneChangeDoorPos(false);
		}
	}
	/*
	BubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	BubbleParticle->SetRelativePos(400.f, 200.f, 0.f);

	CSpecialChangeParticle* SpecialChangeParticle = m_Scene->CreateGameObject<CSpecialChangeParticle>("SpecialChangeParticle");
	SpecialChangeParticle->SetRelativePos(900.f, 200.f, 0.f);
	*/

}

bool CMainScene::Init()
{
	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CTileMapEmpty* TileMapEmpty = m_Scene->LoadGameObject<CTileMapEmpty>();
	TileMapEmpty->LoadByFileName("TileMapText.gobj", OBJECT_PATH);
	TileMapEmpty->SetEditMode(false);

	// Scene?? ?ش? TileMapEmpty?? ???? ?????ε? ?????صд?.
	SetTileMapEmptyObject(TileMapEmpty);

	// TileMap?? ũ?⸦ World Scale?? ?????Ѵ?
	m_Scene->SetWorldResolution(TileMapEmpty->GetWorldScale().x, TileMapEmpty->GetWorldScale().y);

	CBackGround* BackGround = m_Scene->LoadGameObject<CBackGround>();
	BackGround->LoadByFileName("TestBackGround.gobj", OBJECT_PATH);

	CreateSound();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	// CPlayer2D* Player = m_Scene->LoadGameObject<CPlayer2D>();
	// Player->LoadByFileName("TestPlayer.gobj", OBJECT_PATH);

	SetPlayerObject(Player);

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.5f);

	// Sleep(1000); ////

	CPixelTest* Pixel = m_Scene->CreateGameObject<CPixelTest>("Pixel");

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.8f);

	// Sleep(1000);

	// CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	// BubbleParticle->SetRelativePos(400.f, 200.f, 0.f);

	// CSpecialChangeParticle* SpecialChangeParticle = m_Scene->CreateGameObject<CSpecialChangeParticle>("SpecialChangeParticle");
	// SpecialChangeParticle->SetRelativePos(900.f, 200.f, 0.f);

	/*
	BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");

	BubbleParticle->SetRelativePos(-100.f, 0.f, 0.f);

	for (int i = 0; i < 100; ++i)
	{
		BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
		BubbleParticle->SetRelativePos(-100.f + i * 10.f, 0.f, 0.f);
	}

	BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	BubbleParticle->SetRelativePos(-200.f, 0.f, 0.f);

	BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	BubbleParticle->SetRelativePos(-300.f, 0.f, 0.f);
	*/

	// m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");
	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CPlayerHUD>("MainWidget");

	return true;
}

void CMainScene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CMainScene::CreateAnimationSequence()
{
	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerIdle", "Player", TEXT("Player.png"));

	for (int i = 0; i < 7; ++i)
	{
		m_Scene->GetResource()->
		         AddAnimationSequence2DFrame("PlayerIdle", Vector2(i * 50.f, 148.f), Vector2(50.f, 37.f));
	}
}

void CMainScene::CreateSound()
{
	CSceneMode::CreateSound();

	m_Scene->GetResource()->LoadSound("BGM", "BG_Green", 
		true, TEXT("BG_Green.mp3"));
	m_Scene->GetResource()->SoundPlay("BG_Green");
	// m_Scene->GetResource()->LoadSound("UI", "Fire", false, TEXT("Fire1.wav"));
}

void CMainScene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CMainScene::PrepareResources()
{
	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.2f);

	Sleep(500);

	CSceneMode::PrepareResources();

	CreateMaterial();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.6f);

	Sleep(500);

	CreateAnimationSequence();

	CreateParticle();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.85f);

	Sleep(500);

	CreateSound();
}
