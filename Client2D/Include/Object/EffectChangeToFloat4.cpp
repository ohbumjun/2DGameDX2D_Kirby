#include "EffectChangeToFloat4.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/Float4Scene.h"
#include "Player2D.h"
#include "../Object/EffectSceneChangeAlpha.h"
#include "MoonAboveParticle.h"

class CLoadingScene;

CEffectChangeToFloat4::CEffectChangeToFloat4()
{
	SetTypeID<CEffectChangeToFloat4>();
}

CEffectChangeToFloat4::~CEffectChangeToFloat4()
{}

void CEffectChangeToFloat4::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();
	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat4::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat4::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToFloat4::ResetSceneChangeCallbackToPlayer);

}

bool CEffectChangeToFloat4::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat4Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	// 자기 위에 Door Above Particle을 만들어낸다
	CMoonAboveParticle* Particle = m_Scene->CreateGameObject<CMoonAboveParticle>("Particle");
	Particle->SetWorldPos(GetWorldPos().x, GetWorldPos().y + GetWorldScale().y, 0.f);

	return true;
}

void CEffectChangeToFloat4::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat4::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectChangeToFloat4::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToFloat4::ChangeSceneToFloat4Scene);
}

void CEffectChangeToFloat4::ChangeSceneToFloat4Scene()
{
	
	// Next Scene 에 세팅해둔다.
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat4Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Float4_MiddleBoss.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}

void CEffectChangeToFloat4::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToFloat4::MakeSceneChangeEffect);
	}
}

void CEffectChangeToFloat4::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
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

void CEffectChangeToFloat4::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
