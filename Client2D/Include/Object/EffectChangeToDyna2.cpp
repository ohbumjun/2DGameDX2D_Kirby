#include "EffectChangeToDyna2.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/DynaBlade2.h"
#include "../Scene/Float2_1Scene.h"
#include "Player2D.h"
#include "../Object/EffectSceneChangeAlpha.h"
#include "MoonAboveParticle.h"

class CLoadingScene;

CEffectChangeToDyna2::CEffectChangeToDyna2()
{
	SetTypeID<CEffectChangeToDyna2>();
}

CEffectChangeToDyna2::~CEffectChangeToDyna2()
{}

void CEffectChangeToDyna2::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();

	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToDyna2::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToDyna2::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToDyna2::ResetSceneChangeCallbackToPlayer);

	// 자기 위에 Door Above Particle을 만들어낸다
	CMoonAboveParticle* Particle = m_Scene->CreateGameObject<CMoonAboveParticle>("Particle");
	Particle->SetWorldPos(GetWorldPos().x, GetWorldPos().y + GetWorldScale().y, 0.f);
}

bool CEffectChangeToDyna2::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToDyna2Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToDyna2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToDyna2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectChangeToDyna2::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToDyna2::ChangeSceneToDyna2Scene);
}

void CEffectChangeToDyna2::ChangeSceneToDyna2Scene()
{
	
	// Next Scene 에 세팅해둔다.
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CDynaBlade2>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Dyna2_FinalBoss.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}

void CEffectChangeToDyna2::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToDyna2::MakeSceneChangeEffect);
	}
}

void CEffectChangeToDyna2::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
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

void CEffectChangeToDyna2::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
