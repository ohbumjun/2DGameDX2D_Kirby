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

	Sleep(1000);
		
	CreateSound();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.5f);

	Sleep(1000);

	CMonster* Monter = m_Scene->CreateGameObject<CMonster>("Monster");

	CPixelTest* Pixel = m_Scene->CreateGameObject<CPixelTest>("Pixel");

	if (m_LoadingFunction)
		m_LoadingFunction(false, 0.8f);

	Sleep(1000);

	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");
	BubbleParticle->SetRelativePos(-200.f, 0.f, 0.f);


	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CMainWidget>("MainWidget");

	return true;
}

void CMainScene::CreateMaterial()
{
	m_Scene->GetResource()->CreateMaterial<CMaterial>("ParticleMaterial");

	CMaterial* ParticleMaterial = m_Scene->GetResource()->FindMaterial("ParticleMaterial");

	ParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "ParticleTexture", TEXT("Particle/Bubbles99px.png"));

	ParticleMaterial->SetShader("ParticleRenderShader");
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
	CParticle* Bubble = m_Scene->GetResource()->FindParticle("Bubble");

	CMaterial* ParticleMaterial = m_Scene->GetResource()->FindMaterial("ParticleMaterial");

	Bubble->SetMaterial(ParticleMaterial);

	Bubble->SetStartMin(Vector3(-20.f, 20.f, 0.f));
	Bubble->SetStartMax(Vector3(20.f, 20.f, 0.f));

	Bubble->SetScaleMin(Vector3(1.f, 1.f, 1.f));
	Bubble->SetScaleMax(Vector3(50.f, 50.f, 1.f));

	Bubble->SetSpeedMin(100.f);
	Bubble->SetSpeedMax(500.f);

	Bubble->SetLifeTimeMin(1.f);
	Bubble->SetLifeTimeMax(2.f);

	Bubble->SetMove(true);
	Bubble->SetGravity(false);

}
