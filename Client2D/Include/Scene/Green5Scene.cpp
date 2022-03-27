#include "Green5Scene.h"
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
#include "../UI/BossHUD.h"
#include "../UI/PlayerHUD.h"
#include "../Object/LineContainer.h"
#include "GameObject/Line.h"
#include "../Object/EffectSceneChangeAlpha.h"

CGreen5Scene::CGreen5Scene()
{}

CGreen5Scene::~CGreen5Scene()
{}

void CGreen5Scene::Start()
{
	CSceneMode::Start();

	// Scene Change 효과
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(true);

	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	Player2D->SetWorldPos(400.f, 3400.f, 0.f);

	// Main Widget
	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CPlayerHUD>("MainWidget");

	m_Scene->SetPlayerHUD(m_MainWidget);

	// Boss Widget
	m_BossWidget = m_Scene->GetViewPort()->CreateUIWindow<CBossHUD>("BossWidget");

	// 처음에는 안보이게 세팅한다.
	m_BossWidget->Enable(false);

	m_Scene->SetBossHUD(m_BossWidget);

	CBossHUD* BossWidget = (CBossHUD*)m_BossWidget;

	BossWidget->GetIconImage()->SetTexture("TreeBossIcon", TEXT("Project/UI/Icon_Tree.png"));
	BossWidget->GetIconImage()->SetSize(50.f, 50.f);

	BossWidget->GetVSIconImage()->SetTexture("TreeVSBossIcon", TEXT("Project/UI/VsIcon_Tree.png"));
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

bool CGreen5Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CGreen5Scene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CGreen5Scene::CreateAnimationSequence()
{}

void CGreen5Scene::CreateSound()
{}

void CGreen5Scene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CGreen5Scene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
