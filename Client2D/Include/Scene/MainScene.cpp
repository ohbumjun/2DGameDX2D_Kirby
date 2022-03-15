#include "MainScene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "Resource/Particle/Particle.h"
#include "../Object/BubbleParticle.h"
#include "../Object/SpecialChangeParticle.h"
#include "../Object/YellowBird.h"
#include "../Object/BeamMonster.h"
#include "../Object/PurpleBeatle.h"
#include "../Object/NormalBear.h"
#include "../Object/BackGround.h"
#include "../Object/LineContainer.h"

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
	< 아래의 과정들은, PrepareScene 함수에서 미리 처리해준다. >
	
	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
	*/

	// Particle
	CSpecialChangeParticle* SpecialChangeParticle = m_Scene->CreateGameObject<CSpecialChangeParticle>("SpecialChangeParticle");
	SpecialChangeParticle->SetRelativePos(900.f, 200.f, 0.f);

	/*
	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	BubbleParticle->SetRelativePos(400.f, 200.f, 0.f);
	*/

	// TileMapEmpty Type 의 GameObject를 찾는다
	CGameObject* TileMapEmtpyObject = m_Scene->FindGameObjectByTypeID(typeid(CTileMapEmpty).hash_code());

	SetTileMapEmptyObject(TileMapEmtpyObject);

	m_Scene->SetWorldResolution(TileMapEmtpyObject->GetWorldScale().x, TileMapEmtpyObject->GetWorldScale().y);

	CGameObject* Player2D = m_Scene->FindGameObjectByTypeID(typeid(CPlayer2D).hash_code());

	SetPlayerObject(Player2D);

	Player2D->SetWorldPos(100.f, 600.f, 0.f);

	CGameObject* LineContainer = m_Scene->FindGameObjectByTypeID(typeid(CLineContainer).hash_code());

	SetLineContainerObject(LineContainer);

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");
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
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BubbleParticleMaterial");
	CMaterial* BubbleParticleMaterial = m_Scene->GetResource()->FindMaterial("BubbleParticleMaterial");
	BubbleParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "BubbleParticleTexture", TEXT("Particle/Bubbles99px.png"));
	BubbleParticleMaterial->SetShader("ParticleRenderShader");
	BubbleParticleMaterial->SetRenderState("AlphaBlend");


	m_Scene->GetResource()->CreateMaterial<CMaterial>("SpecialChangeMaterial");
	CMaterial* SpecialChangeParticleMaterial = m_Scene->GetResource()->FindMaterial("SpecialChangeMaterial");
	SpecialChangeParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "SpecialChangeParticleTexture", TEXT("Particle/Sparks.png"));
	SpecialChangeParticleMaterial->SetShader("ParticleRenderShader");
	SpecialChangeParticleMaterial->SetRenderState("AlphaBlend");


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
	// BubbleParticle->SetGravity(true);
	BubbleParticle->SetMove(true);

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
}

void CMainScene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
