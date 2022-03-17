#include "FireAttackBackEffect.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/CameraManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ColliderCircle.h"
#include "EffectKirbyRide.h"
#include "Engine.h"
#include "BulletCamera.h"
#include "Monster.h"
#include "UI/UIDamageFont.h"
#include "Component/WidgetComponent.h"

CFireAttackBackEffect::CFireAttackBackEffect()
{
	SetTypeID<CFireAttackBackEffect>();
}

CFireAttackBackEffect::CFireAttackBackEffect(const CFireAttackBackEffect& Beatle) : CGameObject(Beatle)
{}

CFireAttackBackEffect::~CFireAttackBackEffect()
{}

void CFireAttackBackEffect::SetRightAttackDir()
{
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CFireAttackBackEffect::SetLeftAttackDir()
{
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CFireAttackBackEffect::Start()
{
	CGameObject::Start();

	m_Sprite = (CSpriteComponent*)FindComponent("EffectSprite");
}

bool CFireAttackBackEffect::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Sprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BackFireEffect", TEXT("Kirby_Fire_Effect_BackFireEffect.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);

	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CFireAttackBackEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CFireAttackBackEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CFireAttackBackEffect* CFireAttackBackEffect::Clone()
{
	return new CFireAttackBackEffect(*this);
}
