#include "EffectChangeToGreen1.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToGreen1::CEffectChangeToGreen1()
{
	SetTypeID<CEffectChangeToGreen1>();
}

CEffectChangeToGreen1::~CEffectChangeToGreen1()
{}

void CEffectChangeToGreen1::Start()
{
	CGameObject::Start();
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

