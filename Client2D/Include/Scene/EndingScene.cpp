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
#include "../UI/EndingWidget.h"
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

	// Scene Change 효과
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(true);

	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	if (TileMapEmtpyObject)
	{
		m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);
	}

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	// 여기서는 더이상 Player 가 안보여도 된다.
	if (Player2D)
	{
		Player2D->Enable(false);
	}
	
	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);
}

void CEndingScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);
}

bool CEndingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CEndingWidget>("EndingWidget");

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
