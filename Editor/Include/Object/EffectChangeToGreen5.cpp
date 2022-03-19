#include "EffectChangeToGreen5.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToGreen5::CEffectChangeToGreen5()
{
	SetTypeID<CEffectChangeToGreen5>();
}

CEffectChangeToGreen5::CEffectChangeToGreen5(const CEffectChangeToGreen5& Beatle) : CGameObject(Beatle)
{}

CEffectChangeToGreen5::~CEffectChangeToGreen5()
{}

void CEffectChangeToGreen5::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToGreen5::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen5Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToGreen5::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen5::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectChangeToGreen5* CEffectChangeToGreen5::Clone()
{
	return new CEffectChangeToGreen5(*this);
}