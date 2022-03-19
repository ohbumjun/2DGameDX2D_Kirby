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

CFireAttackBackEffect::CFireAttackBackEffect(const CFireAttackBackEffect& Beatle) : CAttackEffect(Beatle)
{}

CFireAttackBackEffect::~CFireAttackBackEffect()
{}

void CFireAttackBackEffect::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);

	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CFireAttackBackEffect::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);

	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CFireAttackBackEffect::Start()
{
	CAttackEffect::Start();

	m_MainSprite = (CSpriteComponent*)FindComponent("EffectSprite");
}

bool CFireAttackBackEffect::Init()
{
	if (!CGameObject::Init())
		return false;

	m_MainSprite = CreateComponent<CSpriteComponent>("EffectSprite");

	SetRootComponent(m_MainSprite);

	m_MainSprite->SetRenderState("AlphaBlend");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BackFireEffect", TEXT("Kirby_Fire_Effect_BackFireEffect.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_MainSprite->SetAnimationInstance(AnimationInstance);

	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");

	m_MainSprite->SetPivot(0.5f, 0.5f, 0.0f);

	return true;
}

void CFireAttackBackEffect::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);
}

void CFireAttackBackEffect::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}

CFireAttackBackEffect* CFireAttackBackEffect::Clone()
{
	return new CFireAttackBackEffect(*this);
}
