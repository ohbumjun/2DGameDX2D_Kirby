#include "EffectChangeToFloat6.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/Float6Scene.h"
#include "Player2D.h"
#include "../Object/EffectSceneChangeAlpha.h"

class CLoadingScene;

CEffectChangeToFloat6::CEffectChangeToFloat6()
{
	SetTypeID<CEffectChangeToFloat6>();
}

CEffectChangeToFloat6::~CEffectChangeToFloat6()
{}

void CEffectChangeToFloat6::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen2Body");
	m_ColliderBody = (CColliderBox2D*)m_RootComponent.Get();

	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat6::ChangeSceneToGreen2Scene);

	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToFloat6::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToFloat6::ResetSceneChangeCallbackToPlayer);

}

bool CEffectChangeToFloat6::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat4Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToFloat6::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat6::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectChangeToFloat6::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToFloat6::ChangeSceneToFloat6Scene);
}

void CEffectChangeToFloat6::ChangeSceneToFloat6Scene()
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
	CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat6Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Float6.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
}

void CEffectChangeToFloat6::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;
		if (!Player)
			return;
		Player->SetSceneChangeCallback(this, &CEffectChangeToFloat6::MakeSceneChangeEffect);
	}
}

void CEffectChangeToFloat6::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
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

void CEffectChangeToFloat6::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
