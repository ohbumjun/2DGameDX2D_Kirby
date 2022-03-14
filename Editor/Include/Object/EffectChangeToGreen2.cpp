#include "EffectChangeToGreen2.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToGreen2::CEffectChangeToGreen2()
{
	SetTypeID<CEffectChangeToGreen2>();
}

CEffectChangeToGreen2::CEffectChangeToGreen2(const CEffectChangeToGreen2& Beatle) : CGameObject(Beatle)
{}

CEffectChangeToGreen2::~CEffectChangeToGreen2()
{}

void CEffectChangeToGreen2::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToGreen2::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen2Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToGreen2::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen2::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectChangeToGreen2* CEffectChangeToGreen2::Clone()
{
	return new CEffectChangeToGreen2(*this);
}