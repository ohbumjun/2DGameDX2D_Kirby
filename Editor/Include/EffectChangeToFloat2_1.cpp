#include "EffectChangeToFloat2_1.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToFloat2_1::CEffectChangeToFloat2_1()
{
	SetTypeID<CEffectChangeToFloat2_1>();
}

CEffectChangeToFloat2_1::~CEffectChangeToFloat2_1()
{}

void CEffectChangeToFloat2_1::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat2_1::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat2_1Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToFloat2_1::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat2_1::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
