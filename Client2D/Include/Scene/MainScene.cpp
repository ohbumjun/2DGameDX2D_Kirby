#include "MainScene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/Monster.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "Resource/Particle/Particle.h"
#include "../Object/BubbleParticle.h"


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

	// Sleep(1000);

	CMonster* Monter = m_Scene->CreateGameObject<CMonster>("Monster");

	CPixelTest* Pixel = m_Scene->CreateGameObject<CPixelTest>("Pixel");

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.8f);

	// Sleep(1000);

	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");

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

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");

	return true;
}

void CMainScene::CreateMaterial()
{
	m_Scene->GetResource()->CreateMaterial<CMaterial>("ParticleMaterial");

	CMaterial* ParticleMaterial = m_Scene->GetResource()->FindMaterial("ParticleMaterial");

	ParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "ParticleTexture", TEXT("Particle/Bubbles99px.png"));

	ParticleMaterial->SetShader("ParticleRenderShader");
	ParticleMaterial->SetRenderState("AlphaBlend");
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
	BubbleParticle->SetSpawnCountMax(100);
	BubbleParticle->SetLifeTimeMin(3.f);
	BubbleParticle->SetLifeTimeMax(5.f);

	BubbleParticle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	BubbleParticle->SetScaleMax(Vector3(80.f, 80.f, 1.f));

	BubbleParticle->SetSpeedMin(200.f);
	BubbleParticle->SetSpeedMax(500.f);

	BubbleParticle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
	BubbleParticle->SetColorMax(Vector4(0.2f, 0.1f, 0.8f, 1.f));

	BubbleParticle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
	BubbleParticle->SetStartMax(Vector3(30.f, 30.f, 0.f));

	BubbleParticle->SetMoveDir(Vector3(0.f, 1.f, 0.f));

	BubbleParticle->SetMove(true);
	BubbleParticle->SetGravity(true);
	BubbleParticle->SetIs2D(true);
	BubbleParticle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));


}
