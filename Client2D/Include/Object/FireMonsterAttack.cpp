#include "FireMonsterAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>
#include "FireMonster.h"
#include "Player2D.h"
#include "Component/ColliderCircle.h"
#include "UI/UIDamageFont.h"

CFireMonsterAttack::CFireMonsterAttack() :
	m_AttackDistLimit(0.f),
	m_AttackDistLimitMax(1000.f)
{}

CFireMonsterAttack::CFireMonsterAttack(const CFireMonsterAttack& Attack) : CAttackEffect(Attack)
{}

CFireMonsterAttack::~CFireMonsterAttack()
{}

void CFireMonsterAttack::SetRightAttackDir(float YDir)
{
	CAttackEffect::SetRightAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
}

void CFireMonsterAttack::SetLeftAttackDir(float YDir)
{
	CAttackEffect::SetLeftAttackDir(YDir);
	m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
}

void CFireMonsterAttack::Start()
{
	CAttackEffect::Start();
}

bool CFireMonsterAttack::Init()
{
	if (!CAttackEffect::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"FireAttackEffect", TEXT("Ability_Fire_AttackEffect.anim"));

	m_MainSprite->SetAnimationInstance(AnimationInstance);
	m_MainSprite->SetWorldScale(80.f, 80.f, 1.f);

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetInfo(Vector2(0.f, 0.f), m_MainSprite->GetWorldScale().x * 0.4f);
	m_Collider->AddCollisionCallback(Collision_State::Begin, (CAttackEffect*)this, &CAttackEffect::MonsterAttackCollisionCallback);

	// m_Collider->AddCollisionCallback(Collision_State::Begin, this, &CFireMonsterAttack::CollisionCallback);

	return true;
}

void CFireMonsterAttack::Update(float DeltaTime)
{
	CAttackEffect::Update(DeltaTime);

	float MoveDist = std::abs(m_AttackDir.x) * DeltaTime * 500.f;

	AddWorldPos(Vector3(m_AttackDir.x, m_AttackDir.y, 0.f) * DeltaTime * 500.f);

	if (m_AttackDistLimit < m_AttackDistLimitMax)
	{
		m_AttackDistLimit += MoveDist;
	}

	if (m_AttackDistLimit >= m_AttackDistLimitMax)
	{
		Destroy();

		m_MonsterOwner->SetAttackEnd();
	}
}

void CFireMonsterAttack::PostUpdate(float DeltaTime)
{
	CAttackEffect::PostUpdate(DeltaTime);
}
