#include "EffectChangeToFloat2_1.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/Float2_1Scene.h"
#include "Player2D.h"
#include "../Object/EffectSceneChangeAlpha.h"
#include "MoonAboveParticle.h"

class CLoadingScene;

CEffectChangeToFloat2_1::CEffectChangeToFloat2_1()
{
	SetTypeID<CEffectChangeToFloat2_1>();
}

CEffectChangeToFloat2_1::~CEffectChangeToFloat2_1()
{}

void CEffectChangeToFloat2_1::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();

	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat2_1::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat2_1::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToFloat2_1::ResetSceneChangeCallbackToPlayer);

	// 자기 위에 Door Above Particle을 만들어낸다
	CMoonAboveParticle* Particle = m_Scene->CreateGameObject<CMoonAboveParticle>("Particle");
	Particle->SetWorldPos(GetWorldPos().x, GetWorldPos().y + GetWorldScale().y, 0.f);
}

bool CEffectChangeToFloat2_1::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen2Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToFloat2_1::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat2_1::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectChangeToFloat2_1::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToFloat2_1::ChangeSceneToFloat2_1Scene);
}

void CEffectChangeToFloat2_1::ChangeSceneToFloat2_1Scene()
{
	/*
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		// Next Scene 에 세팅해둔다.
		CSceneManager::GetInst()->CreateNewScene(false);
		CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen2Scene>(false);
		CSceneManager::GetInst()->GetNextScene()->PrepareResources();
		if (CSceneManager::GetInst()->GetNextScene()->Load("Green2_SpecialScene.scn", SCENE_PATH))
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}
	*/
	// Next Scene 에 세팅해둔다.
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat2_1Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Float2_1.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}

void CEffectChangeToFloat2_1::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToFloat2_1::MakeSceneChangeEffect);
	}
}

void CEffectChangeToFloat2_1::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;

		if (!Player)
			return;

		Player->ResetPlayerCallback();
	}
}

void CEffectChangeToFloat2_1::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
