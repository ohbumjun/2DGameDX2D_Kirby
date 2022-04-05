#include "EndingScene.h"
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

CEndingScene::CEndingScene()
{
	SetTypeID<CEndingScene>();
}

CEndingScene::~CEndingScene()
{}

void CEndingScene::Start()
{
	CSceneMode::Start();

	// Scene Change ȿ��
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(true);

	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	// ���⼭�� ���̻� Player �� �Ⱥ����� �ȴ�.
	Player2D->Enable(false);

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
}

bool CEndingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroWidget>("IntroWidget");

	return true;
}


void CEndingScene::PrepareResources()
{
	CSceneMode::PrepareResources();

	// CreateMaterial();
	
	// CreateParticle();

	CreateSound();
}

void CEndingScene::CreateSound()
{}
