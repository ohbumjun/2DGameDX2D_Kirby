#include "MainScene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "Resource/Particle/Particle.h"
#include "../Object/BubbleParticle.h"
#include "../Object/YellowBird.h"
#include "../Object/BeamMonster.h"

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

	CreateParticle();

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.3f);

	// Sleep(1000);
		
	CreateSound();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.5f);

	// Sleep(1000); //

	CBeamMonster* BeamMonster = m_Scene->CreateGameObject<CBeamMonster>("BeamMonster");
	BeamMonster->SetWorldPos(Vector3(300.f, 400.f, 1.f));

	CYellowBird * YellowBirdMonster = m_Scene->CreateGameObject<CYellowBird>("YellowBird");
	YellowBirdMonster->SetWorldPos(Vector3(-100.f, 500.f, 1.));
	/*
	Monster->LoadAnimationInstance("Normal_YellowBird", TEXT("Normal_YellowBird.anim"));
	Monster->SetCurrentAnimation("RightIdle");
	*/

	CPixelTest* Pixel = m_Scene->CreateGameObject<CPixelTest>("Pixel");

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.8f);

	// Sleep(1000);

	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	BubbleParticle->SetRelativePos(0.f, 200.f, 0.f);
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

	CTileMap* TileMap = m_Scene->CreateGameObject<CTileMap>("TileMapObject");

	return true;
}

void CMainScene::CreateMaterial()
{
	// Particle 
	m_Scene->GetResource()->CreateMaterial<CMaterial>("ParticleMaterial");

	CMaterial* ParticleMaterial = m_Scene->GetResource()->FindMaterial("ParticleMaterial");

	ParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "ParticleTexture", TEXT("Particle/Bubbles99px.png"));

	ParticleMaterial->SetShader("ParticleRenderShader");
	ParticleMaterial->SetRenderState("AlphaBlend");

	// TileMap
	m_Scene->GetResource()->CreateMaterial<CMaterial>("DiabloTileMap");
	CMaterial* TileMapMaterial = m_Scene->GetResource()->FindMaterial("DiabloTileMap");
	TileMapMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "DiabloTile", TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

	TileMapMaterial->SetShader("TileMapShader");
	TileMapMaterial->SetRenderState("AlphaBlend");

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
	m_Scene->GetResource()->CreateParticle("Bubble");
	CParticle* BubbleParticle = m_Scene->GetResource()->FindParticle("Bubble");

	CMaterial* ParticleMaterial = m_Scene->GetResource()->FindMaterial("ParticleMaterial");

	BubbleParticle->SetMaterial(ParticleMaterial);
	BubbleParticle->SetSpawnCountMax(1000);
	BubbleParticle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	BubbleParticle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	BubbleParticle->SetSpeedMin(100.f);
	BubbleParticle->SetSpeedMax(200.f);
	BubbleParticle->SetLifeTimeMin(1.f);
	BubbleParticle->SetLifeTimeMax(8.f);
	BubbleParticle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	BubbleParticle->SetStartMin(Vector3(-300.f, -30.f, 0.f));
	BubbleParticle->SetStartMax(Vector3(300.f, 30.f, 0.f));
	BubbleParticle->SetColorMin(Vector4(0.4f, 0.4f, 0.1f, 1.f));
	BubbleParticle->SetColorMax(Vector4(0.9f, 0.8f, 0.8f, 1.f));
	BubbleParticle->SetMoveAngle(Vector3(0.f, 0.f, -100.f));
	BubbleParticle->SetGravity(true);
	BubbleParticle->SetMove(true);


}
