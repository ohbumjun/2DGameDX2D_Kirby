#include "EffectChangeToGreen1.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/MainScene.h"
#include "../Scene/BeginningScene.h"
#include "../Scene/Green1LoadingScene.h"
#include "Player2D.h"
#include "EffectSceneChangeAlpha.h"

CEffectChangeToGreen1::CEffectChangeToGreen1()
{
	SetTypeID<CEffectChangeToGreen1>();
}

CEffectChangeToGreen1::~CEffectChangeToGreen1()
{}

void CEffectChangeToGreen1::Start()
{
	CGameObject::Start();

	m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen1Body");
	m_ColliderBody = (CColliderBox2D*)(m_RootComponent.Get());

	// m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen1::ChangeSceneToGreen1Scene);
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen1::SetSceneChangeCallbackToPlayer);
	m_ColliderBody->AddCollisionCallback(Collision_State::End, this, &CEffectChangeToGreen1::ResetSceneChangeCallbackToPlayer);
}

bool CEffectChangeToGreen1::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen1Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);


	return true;
}

void CEffectChangeToGreen1::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen1::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectChangeToGreen1::ChangeSceneToGreen1Scene()
{
	// 현재 Scene 이 Beginning Scene 이면 Loading Scene 을 띄우고
	if (m_Scene->GetSceneMode()->CheckType<CBeginningScene>())
	{
		CSceneManager::GetInst()->CreateNewScene();
		CSceneManager::GetInst()->CreateSceneMode<CGreen1LoadingScene>(false);
	}
	// 그게 아니라면 바로 로드 한다.
	else
	{
		CSceneManager::GetInst()->CreateNewScene(false);
		CSceneManager::GetInst()->CreateSceneModeEmpty<CMainScene>(false);
		CSceneManager::GetInst()->GetNextScene()->PrepareResources();
		if (CSceneManager::GetInst()->GetNextScene()->Load("Green1_FirstSpecial.scn", SCENE_PATH))
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}

	/*
	*/
}


void CEffectChangeToGreen1::MakeSceneChangeEffect(const CollisionResult& Result)
{
	CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

	Alpha->SetSceneStart(false);

	Alpha->SetSceneChangeCallback(this, &CEffectChangeToGreen1::ChangeSceneToGreen1Scene);

}

void CEffectChangeToGreen1::SetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;

		Player->SetSceneChangeCallback(this, &CEffectChangeToGreen1::MakeSceneChangeEffect);
	}
}

void CEffectChangeToGreen1::ResetSceneChangeCallbackToPlayer(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject)
	{
		CPlayer2D* Player = (CPlayer2D*)DestObject;

		Player->ResetPlayerCallback();
	}
}

void CEffectChangeToGreen1::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
	m_Scene->SetSceneChangeObject(this);
}
