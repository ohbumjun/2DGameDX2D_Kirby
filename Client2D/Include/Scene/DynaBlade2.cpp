#include "DynaBlade2.h"
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
#include "../UI/BossHUD.h"
#include "../Object/LineContainer.h"
#include "GameObject/Line.h"
#include "../Object/EffectSceneChangeAlpha.h"

CDynaBlade2::CDynaBlade2()
{}

CDynaBlade2::~CDynaBlade2()
{}

void CDynaBlade2::Start()
{
	CSceneMode::Start();

	// Scene Change 효과
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

	// Boss Widget
	m_BossWidget = m_Scene->GetViewPort()->CreateUIWindow<CBossHUD>("BossWidget");

	// 처음에는 안보이게 세팅한다.
	m_BossWidget->Enable(false);

	m_Scene->SetBossHUD(m_BossWidget);

	CBossHUD* BossWidget = (CBossHUD*)m_BossWidget;

	BossWidget->GetIconImage()->SetTexture("DynaBossIcon", TEXT("Project/UI/Icon_Dyna.png"));
	BossWidget->GetIconImage()->SetSize(50.f, 50.f);

	BossWidget->GetVSIconImage()->SetTexture("DynaVSBossIcon", TEXT("Project/UI/VsIcon_Dyna.png"));
	BossWidget->GetVSIconImage()->SetSize(180.f, 80.f);

	BossWidget->SetCallback();

	if (CSceneManager::GetStaticPlayerInfo())
	{
		CPlayer2D* Player = dynamic_cast<CPlayer2D*>(CSceneManager::GetStaticPlayerInfo());

		// 1) PlayerHUD 에 Player HP, MP 세팅하기
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

bool CDynaBlade2::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CDynaBlade2::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CDynaBlade2::CreateAnimationSequence()
{}

void CDynaBlade2::CreateSound()
{
	CSceneMode::CreateSound();
	m_Scene->GetResource()->LoadSound("BGM", "BG_BossDyna",
		true, TEXT("BG_BossDyna.mp3"));
	m_Scene->GetResource()->SoundPlay("BG_BossDyna");

	m_Scene->GetResource()->LoadSound("BGM", "BossDynaBabyMake",
		false, TEXT("BossDynaBabyMake.wav"));
	
}

void CDynaBlade2::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CDynaBlade2::PrepareResources()
{
	// Material을 세팅한다.

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
