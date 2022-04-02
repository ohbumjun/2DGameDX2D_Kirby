#include "BombUltimate.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CBombUltimate::CBombUltimate()  :
m_ExplodeWaitTime(1.f)
{}

CBombUltimate::~CBombUltimate()
{}

void CBombUltimate::Start()
{
	CKirbyAttackEffect::Start();
}

bool CBombUltimate::Init()
{
	if (!CKirbyAttackEffect::Init())
		return false;

	m_MainSprite->SetWorldScale(600.f, 600.f, 1.f);
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);

	m_AttackDistLimitMax = 4100.f;
	m_AttackObjectSpeed = 1000.f;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"KirbyFallBombAttackEffect", TEXT("Ability_Bomb_ThrowBomb.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);

	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectRight")->SetPlayTime(0.1f);
	m_MainSprite->GetAnimationInstance()->FindAnimationSequence2DData("EffectLeft")->SetPlayTime(0.1f);

	m_SideCollisionApplied = false;

	return true;
}

void CBombUltimate::Update(float DeltaTime)
{
	CKirbyAttackEffect::Update(DeltaTime);

	m_ExplodeCurTime += DeltaTime;

	if (m_ExplodeCurTime >= m_ExplodeWaitTime)
	{
		Destroy();

		// 주변에 폭발 효과를 연달아 낸다.
	}
}
void CBombUltimate::PostUpdate(float DeltaTime)
{
	CKirbyAttackEffect::PostUpdate(DeltaTime);
}