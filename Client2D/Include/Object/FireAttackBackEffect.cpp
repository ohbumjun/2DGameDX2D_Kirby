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
}

bool CFireAttackBackEffect::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BackFireEffect", TEXT("Kirby_Fire_Effect_BackFireEffect.anim"));

	// Clone �ؼ� ��������� �Ѵ�.
	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");


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
