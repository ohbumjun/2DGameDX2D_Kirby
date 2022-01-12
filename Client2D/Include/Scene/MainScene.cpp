#include "MainScene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/Monster.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"


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

	CreateSound();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	CMonster* Monter = m_Scene->CreateGameObject<CMonster>("Monster");

	CPixelTest* Pixel = m_Scene->CreateGameObject<CPixelTest>("Pixel");

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");

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
		m_Scene->GetResource()->
		         AddAnimationSequence2DFrame("PlayerIdle", Vector2(i * 50.f, 148.f), Vector2(50.f, 37.f));
	}
}

void CMainScene::CreateSound()
{
	m_Scene->GetResource()->LoadSound("UI", "Teemo", false, TEXT("Skill.wav"));
	m_Scene->GetResource()->LoadSound("UI", "Fire", false, TEXT("Fire1.wav"));
}
