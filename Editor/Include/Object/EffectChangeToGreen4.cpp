#include "EffectChangeToGreen4.h"
#include "Component/ColliderBox2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectChangeToGreen4::CEffectChangeToGreen4()
{
	SetTypeID<CEffectChangeToGreen4>();
}

CEffectChangeToGreen4::CEffectChangeToGreen4(const CEffectChangeToGreen4& Beatle) : CGameObject(Beatle)
{}

CEffectChangeToGreen4::~CEffectChangeToGreen4()
{}

void CEffectChangeToGreen4::Start()
{
	CGameObject::Start();
}

bool CEffectChangeToGreen4::Init()
{
	if (!CGameObject::Init())
		return false;

	m_ColliderBody = CreateComponent<CColliderBox2D>("EffectSceneChangeToGreen4Body");

	SetRootComponent(m_ColliderBody);

	m_ColliderBody->SetCollisionProfile("PlayerEffect");
	m_ColliderBody->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CEffectChangeToGreen4::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectChangeToGreen4::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectChangeToGreen4* CEffectChangeToGreen4::Clone()
{
	return new CEffectChangeToGreen4(*this);
}