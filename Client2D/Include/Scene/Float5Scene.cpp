#include "Float5Scene.h"
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
#include "../UI/PlayerHUD.h"
#include "../Object/LineContainer.h"
#include "../Object/EffectSceneChangeAlpha.h"

CFloat5Scene::CFloat5Scene()
{}

CFloat5Scene::~CFloat5Scene()
{}

void CFloat5Scene::Start()
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

	Player2D->SetWorldPos(100.f, 400.f, 0.f);

	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);

	// Main Widget
	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CPlayerHUD>("MainWidget");

	m_Scene->SetPlayerHUD(m_MainWidget);

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

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, 0.f);
		}
	}
}

bool CFloat5Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CFloat5Scene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CFloat5Scene::CreateAnimationSequence()
{}

void CFloat5Scene::CreateSound()
{}

void CFloat5Scene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CFloat5Scene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
