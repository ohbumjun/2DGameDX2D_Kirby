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

CGreen3Scene::CGreen3Scene()
{}

CGreen3Scene::~CGreen3Scene()
{}

void CGreen3Scene::Start()
{
	CSceneMode::Start();

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

void CGreen3Scene::CreateAnimationSequence()
{}

void CGreen3Scene::CreateSound()
{}

void CGreen3Scene::CreateParticle()
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

void CGreen3Scene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CreateSound();
}
