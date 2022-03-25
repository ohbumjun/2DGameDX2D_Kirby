#include "Green3Scene.h"
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

CGreen3Scene::CGreen3Scene()
{}

CGreen3Scene::~CGreen3Scene()
{}

void CGreen3Scene::Start()
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

	if (Player2D)
	{
		Player2D->SetWorldPos(100.f, 500.f, 0.f);
	}

	// Line 세팅 
	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	// LineContainer 는 없지만, 현재 Scene에 Line이 존재한다면
	if (LineContainer == nullptr && m_Scene->FindGameObjectByTypeID(typeid(CLine).hash_code()))
	{
		// 없다면, 새롭게 Line Container Game Object 을 만들어준 다음
		// Line Container 자식으로 넣어준다.
		LineContainer = CSceneManager::GetInst()->GetScene()->CreateGameObject<CLineContainer>("LineContainer");

		LineContainer->AddChildGameObject(m_Scene->FindGameObjectByTypeID(typeid(CLine).hash_code()));
	}

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

bool CGreen3Scene::Init()
{
	if (!CSceneMode::Init())
		return false;

	return true;
}

void CGreen3Scene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CGreen3Scene::CreateAnimationSequence()
{}

void CGreen3Scene::CreateSound()
{}

void CGreen3Scene::CreateParticle()
{
	CSceneMode::CreateParticle();
}

void CGreen3Scene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
