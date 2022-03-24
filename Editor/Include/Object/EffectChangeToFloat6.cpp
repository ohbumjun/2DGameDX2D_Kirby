#include "EffectChangeToFloat6.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToFloat6::CEffectChangeToFloat6()
{
	SetTypeID<CEffectChangeToFloat6>();
}

CEffectChangeToFloat6::~CEffectChangeToFloat6()
{}

void CEffectChangeToFloat6::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat6::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat6Body");

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
