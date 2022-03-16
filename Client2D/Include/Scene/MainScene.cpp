#include "MainScene.h"
#include "../UI/MainWidget.h"
#include "Resource/Particle/Particle.h"
// Scene
#include <Scene/SceneManager.h>
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/ViewPort.h"
// Object
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
#include "../Object/BeamMonsterAttack.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

void CMainScene::Start()
{
	CSceneMode::Start();

	/*
	CBeamMonsterAttack* Attack = m_Scene->CreateGameObject<CBeamMonsterAttack>("Attack");
	Attack->SetWorldPos(400.f, 600.f, 1.f);
	*/

	/*
	< 아래의 과정들은, PrepareScene 함수에서 미리 처리해준다. >
	
	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
	*/

	// TileMapEmpty Type 의 GameObject를 찾는다
	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	// Player
	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	// Line
	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");

	if (CSceneManager::GetStaticPlayerInfo())
	{
		if (m_Scene->GetSceneChangeObject())
		{
			Vector3 PlayerSpawnBasePos = m_Scene->GetSceneChangeObject()->GetWorldPos();

			Player2D->SetWorldPos(PlayerSpawnBasePos.x, PlayerSpawnBasePos.y + 100.f, PlayerSpawnBasePos.z);
		}
	}

	// 
	CGameObject* DoorToGreen2Scene = m_Scene->FindGameObjectByTypeID(typeid(CEffectChangeToGreen2).hash_code());

	if (DoorToGreen2Scene)
	{
		CMoonAboveParticle* MoonAboveParticle = m_Scene->CreateGameObject<CMoonAboveParticle>("MoonAboveParticle");

		MoonAboveParticle->SetWorldPos(
			DoorToGreen2Scene->GetWorldPos().x, 
			DoorToGreen2Scene->GetWorldPos().y + DoorToGreen2Scene->GetWorldScale().y * 2.f,
			DoorToGreen2Scene->GetWorldPos().z);
	}
	/*
	
	 *	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
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

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.3f);


	// CTileMapEmpty* TileMapEmpty = m_Scene->CreateGameObject<CTileMapEmpty>("TileMapEmptyObject");
	CTileMapEmpty* TileMapEmpty = m_Scene->LoadGameObject<CTileMapEmpty>();
	TileMapEmpty->LoadByFileName("TileMapText.gobj", OBJECT_PATH);
	TileMapEmpty->SetEditMode(false);

	// Scene에 해당 TileMapEmpty를 멤버 변수로도 세팅해둔다.
	SetTileMapEmptyObject(TileMapEmpty);

	// TileMap의 크기를 World Scale로 세팅한다
	m_Scene->SetWorldResolution(TileMapEmpty->GetWorldScale().x, TileMapEmpty->GetWorldScale().y);

	CBackGround* BackGround = m_Scene->LoadGameObject<CBackGround>();
	BackGround->LoadByFileName("TestBackGround.gobj", OBJECT_PATH);

	// Sleep(1000);
		
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

	CSpecialChangeParticle* SpecialChangeParticle = m_Scene->CreateGameObject<CSpecialChangeParticle>("SpecialChangeParticle");
	SpecialChangeParticle->SetRelativePos(900.f, 200.f, 0.f);

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

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");

	return true;
}

void CMainScene::CreateMaterial()
{
	// Particle
	// - Bubble
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BubbleParticleMaterial");
	CMaterial* BubbleParticleMaterial = m_Scene->GetResource()->FindMaterial("BubbleParticleMaterial");
	BubbleParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "BubbleParticleTexture", TEXT("Particle/Bubbles99px.png"));
	BubbleParticleMaterial->SetShader("ParticleRenderShader");
	BubbleParticleMaterial->SetRenderState("AlphaBlend");

	// - SpecialChange
	m_Scene->GetResource()->CreateMaterial<CMaterial>("SpecialChangeMaterial");
	CMaterial* SpecialChangeParticleMaterial = m_Scene->GetResource()->FindMaterial("SpecialChangeMaterial");
	SpecialChangeParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "SpecialChangeParticleTexture", TEXT("Particle/Sparks.png"));
	SpecialChangeParticleMaterial->SetShader("ParticleRenderShader");
	SpecialChangeParticleMaterial->SetRenderState("AlphaBlend");

	// - MoonAbove
	m_Scene->GetResource()->CreateMaterial<CMaterial>("MoonAboveParticleMaterial");
	CMaterial* MoonAboveParticleMaterial = m_Scene->GetResource()->FindMaterial("MoonAboveParticleMaterial");
	MoonAboveParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "MoonAboveParticleTexture", TEXT("Particle/Bubbles99px.png"));
	MoonAboveParticleMaterial->SetShader("ParticleRenderShader");
	MoonAboveParticleMaterial->SetRenderState("AlphaBlend");


	// TileMap
	m_Scene->GetResource()->CreateMaterial<CMaterial>("DiabloTileMap");
	CMaterial* TileMapMaterial = m_Scene->GetResource()->FindMaterial("DiabloTileMap");
	TileMapMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "DiabloTile", TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

	TileMapMaterial->SetShader("TileMapShader");
	TileMapMaterial->SetRenderState("AlphaBlend");

	// Back Material
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BackMaterial");
	CMaterial* BackMaterial = m_Scene->GetResource()->FindMaterial("BackMaterial");
	BackMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "SampleBack", TEXT("LoadingBack.jpg"));

	BackMaterial->SetShader("Mesh2DShader");

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

void CMainScene::CreateParticle()
{
	// Bubble
	m_Scene->GetResource()->CreateParticle("Bubble");
	CParticle* BubbleParticle = m_Scene->GetResource()->FindParticle("Bubble");
	CMaterial* ParticleMaterial = m_Scene->GetResource()->FindMaterial("BubbleParticleMaterial");

	BubbleParticle->SetMaterial(ParticleMaterial);
	BubbleParticle->SetSpawnCountMax(50);
	BubbleParticle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	BubbleParticle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	BubbleParticle->SetSpeedMin(100.f);
	BubbleParticle->SetSpeedMax(300.f);
	BubbleParticle->SetLifeTimeMin(0.6f);
	BubbleParticle->SetLifeTimeMax(1.3f);
	BubbleParticle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	BubbleParticle->SetStartMin(Vector3(-100.f, 0.f, 0.f));
	BubbleParticle->SetStartMax(Vector3(100.f, 10.f, 0.f));
	BubbleParticle->SetColorMin(Vector4(0.6f, 0.8f, 1.0f, 1.f));
	BubbleParticle->SetColorMax(Vector4(0.1f, 0.1f, 1.0f, 1.f));
	BubbleParticle->SetMoveAngle(Vector3(0.f, 0.f, -40.f));
	BubbleParticle->SetGravity(true);
	// BubbleParticle->SetMove(true);

	// Special Change
	m_Scene->GetResource()->CreateParticle("SpecialChange");
	CParticle* SpecialChangeParticle = m_Scene->GetResource()->FindParticle("SpecialChange");
	CMaterial* SpecialChangeParticleMaterial = m_Scene->GetResource()->FindMaterial("SpecialChangeMaterial");

	SpecialChangeParticle->SetMaterial(ParticleMaterial);
	SpecialChangeParticle->SetSpawnCountMax(100);
	SpecialChangeParticle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	SpecialChangeParticle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	SpecialChangeParticle->SetSpeedMin(50.f);
	SpecialChangeParticle->SetSpeedMax(400.f);
	SpecialChangeParticle->SetLifeTimeMin(0.6f);
	SpecialChangeParticle->SetLifeTimeMax(1.3f);
	SpecialChangeParticle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	SpecialChangeParticle->SetStartMin(Vector3(-200.f, 0.f, 0.f));
	SpecialChangeParticle->SetStartMax(Vector3(200.f, 10.f, 0.f));
	SpecialChangeParticle->SetColorMin(Vector4(0.6f, 0.8f, 1.0f, 1.f));
	SpecialChangeParticle->SetColorMax(Vector4(0.1f, 0.1f, 1.0f, 1.f));
	SpecialChangeParticle->SetMoveAngle(Vector3(0.f, 0.f, -50.f));
	// BubbleParticle->SetGravity(true);
	SpecialChangeParticle->SetMove(true);

	// MoonAbove
	m_Scene->GetResource()->CreateParticle("MoonAbove");
	CParticle* MoonAboveParticle = m_Scene->GetResource()->FindParticle("MoonAbove");
	CMaterial* MoonAboveParticleMaterial = m_Scene->GetResource()->FindMaterial("MoonAboveParticleMaterial");

	MoonAboveParticle->SetMaterial(MoonAboveParticleMaterial);
	MoonAboveParticle->SetSpawnCountMax(100);
	MoonAboveParticle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	MoonAboveParticle->SetScaleMax(Vector3(40.f, 40.f, 1.f));
	MoonAboveParticle->SetSpeedMin(5.f);
	MoonAboveParticle->SetSpeedMax(100.f);
	MoonAboveParticle->SetLifeTimeMin(0.2f);
	MoonAboveParticle->SetLifeTimeMax(3.5f);
	MoonAboveParticle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	MoonAboveParticle->SetStartMin(Vector3(-150.f, 0.f, 0.f));
	MoonAboveParticle->SetStartMax(Vector3(150.f, 10.f, 0.f));
	MoonAboveParticle->SetColorMin(Vector4(0.6f, 1.0f, 0.6f, 1.f));
	MoonAboveParticle->SetColorMax(Vector4(0.1f, 1.0f, 0.1f, 1.f));
	MoonAboveParticle->SetMoveAngle(Vector3(0.f, 0.f, -20.f));
	MoonAboveParticle->SetGravity(true);
	// SpecialChangeParticle->SetMove(true);
}

void CMainScene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
