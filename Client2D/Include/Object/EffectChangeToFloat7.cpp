#include "EffectChangeToFloat7.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/Float7Scene.h"
#include "Player2D.h"
#include "../Object/EffectSceneChangeAlpha.h"

class CLoadingScene;

CEffectChangeToFloat7::CEffectChangeToFloat7()
{
	SetTypeID<CEffectChangeToFloat7>();
}

CEffectChangeToFloat7::~CEffectChangeToFloat7()
{}

void CEffectChangeToFloat7::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();

	m_ColliderBody->SetCollisionProfile("Monster");
	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat7::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat7::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToFloat7::ResetSceneChangeCallbackToPlayer);

}

bool CEffectChangeToFloat7::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat7Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToFloat7::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat7::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectChangeToFloat7::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToFloat7::ChangeSceneToFloat7Scene);
}

void CEffectChangeToFloat7::ChangeSceneToFloat7Scene()
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
	CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat7Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Float7_SecondBoss.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}

void CEffectChangeToFloat7::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToFloat7::MakeSceneChangeEffect);
	}
}

void CEffectChangeToFloat7::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
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

void CEffectChangeToFloat7::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
