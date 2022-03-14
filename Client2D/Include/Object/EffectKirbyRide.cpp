#include "EffectKirbyRide.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"

CEffectKirbyRide::CEffectKirbyRide() 
{
	SetTypeID<CEffectKirbyRide>();
}

CEffectKirbyRide::CEffectKirbyRide(const CEffectKirbyRide& Beatle)
{}

CEffectKirbyRide::~CEffectKirbyRide()
{}

void CEffectKirbyRide::Start()
{
	CGameObject::Start();
}

bool CEffectKirbyRide::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbyRideEffect", TEXT("Effect_KirbyRide.anim"));

	if (!AnimationInstance)
	{
		assert(false);
		return false;
	}

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	SetWorldScale(GetWorldScale().x * 0.7f, GetWorldScale().y * 0.7f, 1.f);

	return true;
}

void CEffectKirbyRide::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CEffectKirbyRide::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CEffectKirbyRide* CEffectKirbyRide::Clone()
{
	return new CEffectKirbyRide(*this);
}
