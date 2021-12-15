
#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneResource.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CreateMaterial();

	CreateAnimationSequence();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	return true;
}

void CMainScene::CreateMaterial()
{
}

void CMainScene::CreateAnimationSequence()
{
	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerIdle", "Player", TEXT("Player.png"));

	for (int i = 0; i < 7; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerIdle", Vector2(i * 50.f, 148.f), Vector2(50.f, 37.f));
	}
}
