#include "Fish.h"
#include "BeamMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>

#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CFish::CFish()
{
	SetTypeID<CFish>();

	m_DashDistance = 550.f;

	m_AttackDistance = 200.f;
}


CFish::~CFish()
{
}

void CFish::Start()
{
	CWaterMonster::Start();

	m_PhysicsSimulate = false;

	m_IsGround = true;

	// m_IsGroundObject = true;

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_IsAttacking = false;
}

bool CFish::Init()
{
	if (!CWaterMonster::Init())
		return false;

	// LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	// SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CFish::Update(float DeltaTime)
{
	CWaterMonster::Update(DeltaTime);
}

void CFish::PostUpdate(float DeltaTime)
{
	CWaterMonster::PostUpdate(DeltaTime);
}

void CFish::ChangeIdleAnimation()
{
	CMonster::ChangeTraceAnimation();
}

void CFish::Attack()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	m_IsAttacking = true;

	m_AttackLimitTime = m_AttackLimitTimeMax;

}
