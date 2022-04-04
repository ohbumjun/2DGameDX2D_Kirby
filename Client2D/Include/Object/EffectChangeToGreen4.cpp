#include "EffectChangeToGreen4.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/Green4Scene.h"
#include "Player2D.h"
#include "../Object/EffectSceneChangeAlpha.h"
#include "MoonAboveParticle.h"

class CLoadingScene;

CEffectChangeToGreen4::CEffectChangeToGreen4()
{
	SetTypeID<CEffectChangeToGreen4>();
}

CEffectChangeToGreen4::CEffectChangeToGreen4(const CEffectChangeToGreen4& Beatle)
{}

CEffectChangeToGreen4::~CEffectChangeToGreen4()
{}

void CEffectChangeToGreen4::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();

	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen4::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen4::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToGreen4::ResetSceneChangeCallbackToPlayer);

	// 자기 위에 Door Above Particle을 만들어낸다
	CMoonAboveParticle* Particle = m_Scene->CreateGameObject<CMoonAboveParticle>("Particle");
	Particle->SetWorldPos(GetWorldPos().x, GetWorldPos().y + GetWorldScale().y, 0.f);
}

bool CEffectChangeToGreen4::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen4Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToGreen4::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen4::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectChangeToGreen4* CEffectChangeToGreen4::Clone()
{
	return new CEffectChangeToGreen4(*this);
}

void CEffectChangeToGreen4::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToGreen4::ChangeSceneToGreen4Scene);
}

void CEffectChangeToGreen4::ChangeSceneToGreen4Scene()
{
	
	// Next Scene 에 세팅해둔다.
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen4Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Green4.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}


void CEffectChangeToGreen4::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToGreen4::MakeSceneChangeEffect);
	}
}

void CEffectChangeToGreen4::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
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

void CEffectChangeToGreen4::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	// m_Scene->SetSceneChangeObject(this);
}
