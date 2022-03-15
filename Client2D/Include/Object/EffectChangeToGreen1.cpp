#include "EffectChangeToGreen1.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Scene/MainScene.h"

CEffectChangeToGreen1::CEffectChangeToGreen1()
{
	SetTypeID<CEffectChangeToGreen1>();
}

CEffectChangeToGreen1::CEffectChangeToGreen1(const CEffectChangeToGreen1& Beatle)
{}

CEffectChangeToGreen1::~CEffectChangeToGreen1()
{}

void CEffectChangeToGreen1::Start()
{
	CGameObject::Start();

	// m_ColliderBody = (CColliderBox2D*)FindComponent("EffectSceneChangeToGreen1Body");
	m_ColliderBody = (CColliderBox2D*)(m_RootComponent.Get());

	if (!m_ColliderBody)
	{
		dynamic_cast<CColliderBox2D*>(m_RootComponent.Get())->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen1::ChangeSceneToGreen1Scene);
	}
	else
	{
		m_ColliderBody->AddCollisionCallback(Collision_State::Begin, this, &CEffectChangeToGreen1::ChangeSceneToGreen1Scene);
	}

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

CEffectChangeToGreen1* CEffectChangeToGreen1::Clone()
{
	return new CEffectChangeToGreen1(*this);
}

void CEffectChangeToGreen1::ChangeSceneToGreen1Scene(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (m_Scene->GetPlayerObject() == DestObject )
	{
		// Next Scene 에 세팅해둔다.
		CSceneManager::GetInst()->CreateNewScene(false);
		CSceneManager::GetInst()->CreateSceneModeEmpty<CMainScene>(false);
		CSceneManager::GetInst()->GetNextScene()->PrepareResources();
		if (CSceneManager::GetInst()->GetNextScene()->Load("Green1.scn", SCENE_PATH))
		{
			CSceneManager::GetInst()->ChangeNextScene();
		}
	}

}