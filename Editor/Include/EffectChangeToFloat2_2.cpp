#include "EffectChangeToFloat2_2.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToFloat2_2::CEffectChangeToFloat2_2()
{
	SetTypeID<CEffectChangeToFloat2_2>();
}

CEffectChangeToFloat2_2::~CEffectChangeToFloat2_2()
{}

void CEffectChangeToFloat2_2::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat2_2::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat2_2Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToFloat2_2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat2_2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
