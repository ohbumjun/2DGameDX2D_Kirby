#include "EffectChangeToFloat7.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToFloat7::CEffectChangeToFloat7()
{
	SetTypeID<CEffectChangeToFloat7>();
}

CEffectChangeToFloat7::~CEffectChangeToFloat7()
{}

void CEffectChangeToFloat7::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToFloat7::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToFloat7Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("Monster");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToFloat7::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToFloat7::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
