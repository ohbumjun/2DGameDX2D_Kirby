#include "EffectChangeToFloat3.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToFloat3::CEffectChangeToFloat3()
{
	SetTypeID<CEffectChangeToFloat3>();
}

CEffectChangeToFloat3::~CEffectChangeToFloat3()
{}

void CEffectChangeToFloat3::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat3::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat3Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToFloat3::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat3::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
