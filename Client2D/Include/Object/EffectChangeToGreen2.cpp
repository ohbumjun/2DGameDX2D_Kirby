#include "EffectChangeToGreen2.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/Green2Scene.h"
#include "Player2D.h"

class CLoadingScene;

CEffectChangeToGreen2::CEffectChangeToGreen2() 
{
	SetTypeID<CEffectChangeToGreen2>();
}

CEffectChangeToGreen2::CEffectChangeToGreen2(const CEffectChangeToGreen2& Beatle)
{}

CEffectChangeToGreen2::~CEffectChangeToGreen2()
{}

void CEffectChangeToGreen2::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();

	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen2::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen2::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToGreen2::ResetSceneChangeCallbackToPlayer);

}

bool CEffectChangeToGreen2::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen2Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToGreen2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectChangeToGreen2* CEffectChangeToGreen2::Clone()
{
	return new CEffectChangeToGreen2(*this);
}

void CEffectChangeToGreen2::ChangeSceneToGreen2Scene(const CollisionResult& Result)
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
	CSceneManager::GetInst()->CreateSceneModeEmpty<CGreen2Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Green2_SpecialScene.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}

void CEffectChangeToGreen2::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToGreen2::ChangeSceneToGreen2Scene);
	}
}

void CEffectChangeToGreen2::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D*Player = (CPlayer2D*)DestObject;

		if (!Player)
			return;

		Player->ResetPlayerCallback();
	}
}
