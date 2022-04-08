#include "SceneMode.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"
#include "../GameObject/Line.h"
#include "../Resource/Material/Material.h"

CSceneMode::CSceneMode() :
m_Scene(nullptr),
m_PlayerObject(nullptr),
m_IsEditMode(false)
{
	SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::SetPlayerObject(CGameObject* Obj)
{
	m_PlayerObject = Obj;
}

void CSceneMode::SetTileMapEmptyObject(CGameObject* Obj)
{
	m_TileMapEmptyObject = Obj;
}

void CSceneMode::SetLineContainerObject(CGameObject* Obj)
{
	m_LineContainerObject = Obj;
}

void CSceneMode::SetSceneChangeObject(CGameObject* Obj)
{
	m_SceneChangeObject = Obj;
}

void CSceneMode::CreateMaterial()
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
	// SpecialChangeParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "SpecialChangeParticleTexture", TEXT("Particle/HardRain.png"));
	SpecialChangeParticleMaterial->SetShader("ParticleRenderShader");
	SpecialChangeParticleMaterial->SetRenderState("AlphaBlend");

	// - MoonAbove
	m_Scene->GetResource()->CreateMaterial<CMaterial>("MoonAboveParticleMaterial");
	CMaterial* MoonAboveParticleMaterial = m_Scene->GetResource()->FindMaterial("MoonAboveParticleMaterial");
	MoonAboveParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "MoonAboveParticleTexture", TEXT("Particle/Bubbles99px.png"));
	MoonAboveParticleMaterial->SetShader("ParticleRenderShader");
	MoonAboveParticleMaterial->SetRenderState("AlphaBlend");

	// - Boss Fight
	m_Scene->GetResource()->CreateMaterial<CMaterial>("BossFightMaterial");
	CMaterial* BossFightParticleMaterial = m_Scene->GetResource()->FindMaterial("BossFightMaterial");
	BossFightParticleMaterial->AddTexture(0,
		(int)Buffer_Shader_Type::Pixel, "BossFightParticleTexture", TEXT("Particle/HardRain.png"));
	BossFightParticleMaterial->SetShader("ParticleRenderShader");
	BossFightParticleMaterial->SetRenderState("AlphaBlend");

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

void CSceneMode::CreateParticle()
{
	// Boss Fight
	// Boss 가 만들어낼 것이다.
	m_Scene->GetResource()->CreateParticle("BossFight");
	CParticle* BossFightParticle = m_Scene->GetResource()->FindParticle("BossFight");
	CMaterial* BossFightParticleMaterial = m_Scene->GetResource()->FindMaterial("BossFightMaterial");

	BossFightParticle->SetMaterial(BossFightParticleMaterial);
	BossFightParticle->SetSpawnCountMax(6000);
	BossFightParticle->SetScaleMin(Vector3(30.f, 30.f, 1.f));
	BossFightParticle->SetScaleMax(Vector3(80.f, 80.f, 1.f));
	BossFightParticle->SetSpeedMin(1200.f);
	BossFightParticle->SetSpeedMax(1600.f);
	BossFightParticle->SetLifeTimeMin(1.0f);
	BossFightParticle->SetLifeTimeMax(5.0f);
	BossFightParticle->SetMoveDir(Vector3(1.f, -1.f, 0.f));
	BossFightParticle->SetStartMin(Vector3(-1000.f, 300.f, 0.f));
	BossFightParticle->SetStartMax(Vector3(000.f, 4000.f, 0.f));
	BossFightParticle->SetColorMin(Vector4(0.5f, 0.4f, 0.4f, 1.f));
	BossFightParticle->SetColorMax(Vector4(0.9f, 0.2f, 0.2f, 1.f));
	BossFightParticle->SetMoveAngle(Vector3(0.f, 0.f, -50.f));
	// BubbleParticle->SetGravity(true);
	BossFightParticle->SetMove(true);

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

	SpecialChangeParticle->SetMaterial(SpecialChangeParticleMaterial);
	SpecialChangeParticle->SetSpawnCountMax(500);
	SpecialChangeParticle->SetScaleMin(Vector3(10.f, 10.f, 1.f));
	SpecialChangeParticle->SetScaleMax(Vector3(30.f, 30.f, 1.f));
	SpecialChangeParticle->SetSpeedMin(700.f);
	SpecialChangeParticle->SetSpeedMax(1200.f);
	SpecialChangeParticle->SetLifeTimeMin(0.1f);
	SpecialChangeParticle->SetLifeTimeMax(0.3f);
	SpecialChangeParticle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	SpecialChangeParticle->SetStartMin(Vector3(-20.f, -50.f, 0.f));
	SpecialChangeParticle->SetStartMax(Vector3(20.f, 0.f, 0.f));
	SpecialChangeParticle->SetColorMin(Vector4(1.0f, 0.4f, 0.4f, 1.f));
	SpecialChangeParticle->SetColorMax(Vector4(1.0f, 0.0f, 0.0f, 1.f));
	SpecialChangeParticle->SetMoveAngle(Vector3(0.f, 0.f, -90.f));
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

void CSceneMode::Start()
{
	if (m_Scene->GetPlayerObject())
	{
		m_Scene->GetPlayerObject()->Enable(true);
	}
}

bool CSceneMode::Init()
{
	return true;
}

void CSceneMode::Update(float DeltaTime)
{
}

void CSceneMode::PostUpdate(float DeltaTime)
{
}

void CSceneMode::PrepareResources()
{
	// 기본 Material 생성해두기
	m_Scene->GetResource()->CreateMaterial<CMaterial>("Color");

	CSharedPtr<CMaterial> Mtrl = m_Scene->GetResource()->FindMaterial("Color");

	Mtrl->SetShader("ColorMeshShader");

	m_Scene->GetResource()->CreateMaterial<CMaterial>("BaseTexture");

	Mtrl = m_Scene->GetResource()->FindMaterial("BaseTexture");

	Mtrl->SetShader("Mesh2DShader");

	CTexture* Texture = CResourceManager::GetInst()->FindTexture("EngineTexture");

	if (Mtrl->IsTextureEmpty())
	{
		Mtrl->AddTexture(0, static_cast<int>(Buffer_Shader_Type::Pixel), "EngineTexture", Texture);
	}
}

void CSceneMode::CreateSound()
{
	// Scene
	m_Scene->GetResource()->LoadSound("Scene",
		"SceneChange", false, TEXT("SceneStart.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"SceneChangeStar", false, TEXT("SceneChangeStar.wav"));

	// Player
	// true
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"WaterSwim", true, TEXT("WaterSwim.mp3"));

	// false
	m_Scene->GetResource()->LoadSound("PlayerEffect", 
		"PlayerSpitOut", false, TEXT("PlayerSpitout.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerSpecialAttack", false, TEXT("PlayerSpecialAttack.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerSlide", false, TEXT("PlayerSlid.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerPull", false, TEXT("PlayerPull.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerJump", false, TEXT("PlayerJump.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerNormalAttack", false, TEXT("PlayerNormalAttack.wav")); //
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerUltimateAttack", false, TEXT("PlayerUltimateAttack.wav")); //
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerGoUpAttack", false, TEXT("PlayerGoUpAttack.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerGetOutOfWater", false, TEXT("PlayerGetOutOfWater.mp3"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerFly", false, TEXT("PlayerFly.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerFallDownAttack", false, TEXT("PlayerFallDownAttack.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEnterWater", false, TEXT("PlayerEnterWater.mp3"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerDeath", false, TEXT("PlayerDeath.mp3"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerDash", false, TEXT("PlayerDash.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerDamageMonster", false, TEXT("PlayerDamageMonster.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerDamaged", false, TEXT("PlayerDamaged.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerChangeSceneStart", false, TEXT("PlayerChangeSceneStart.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerChange", false, TEXT("PlayerChange.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerItemGet", false, TEXT("Item_Get.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerCloneEffect", false, TEXT("PlayerCloneEffect.wav"));
	
	/*
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	m_Scene->GetResource()->LoadSound("PlayerEffect",
		"PlayerEat", false, TEXT("PlayerEat.wav"));
	*/


	// Monster
	m_Scene->GetResource()->LoadSound("Monster",
		"AbilityAttack", false, TEXT("SpecialAttack.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"ScissorStartJump", false, TEXT("ScissorStartJump.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"MonsterDeath", false, TEXT("MonsterDeath.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"MonsterTraceStart", false, TEXT("Monster_Trace_Start.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"BossJump", false, TEXT("BossJump.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"BossCloseAttack", false, TEXT("Boss_CloseAttack.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"BossFarAttack", false, TEXT("BossFarAttack.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"BossWorldResolutionEnd", false, TEXT("BossWorldResolutionEnd.wav"));
	m_Scene->GetResource()->LoadSound("Monster",
		"BG_BossFightAngry", true, TEXT("BG_BossFightAngry.mp3"));
}
