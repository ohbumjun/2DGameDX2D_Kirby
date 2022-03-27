#include "EffectChangeToFloat4.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToFloat4::CEffectChangeToFloat4()
{
	SetTypeID<CEffectChangeToFloat4>();
}

CEffectChangeToFloat4::~CEffectChangeToFloat4()
{}

void CEffectChangeToFloat4::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat4::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat4Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");

	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

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
