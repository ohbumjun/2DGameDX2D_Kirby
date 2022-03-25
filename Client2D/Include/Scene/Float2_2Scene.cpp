#include "Float2_2Scene.h"
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

CFloat2_2Scene::CFloat2_2Scene()
{}

CFloat2_2Scene::~CFloat2_2Scene()
{}

void CFloat2_2Scene::Start()
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

	Player2D->SetWorldPos(450.f, 1200.f, 0.f);

	m_Scene->GetCameraManager()->GetCurrentCamera()->SetWorldPos(Player2D->GetWorldPos());

	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);
}

bool CFloat2_2Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CFloat2_2Scene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CFloat2_2Scene::CreateAnimationSequence()
{}

void CFloat2_2Scene::CreateSound()
{}

void CFloat2_2Scene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CFloat2_2Scene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
