#include "Squid.h"
#include "BeamMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>

#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CSquid::CSquid()
{
	SetTypeID<CSquid>();

	m_DashDistance = 550.f;

	m_AttackDistance = 450.f;
}

CSquid::CSquid(const CSquid& Monster) : CWaterMonster(Monster)
{}

CSquid::~CSquid()
{}

void CSquid::Start()
{
	CWaterMonster::Start();

	m_PhysicsSimulate = false;

	m_IsGround = true;

	// m_IsGroundObject = true;

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_IsAttacking = false;
}

bool CSquid::Init()
{
	if (!CWaterMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CSquid::Update(float DeltaTime)
{
	CWaterMonster::Update(DeltaTime);
}

void CSquid::PostUpdate(float DeltaTime)
{
	CWaterMonster::PostUpdate(DeltaTime);
}

CSquid* CSquid::Clone()
{
	return new CSquid(*this);
}

void CSquid::Attack()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	m_IsAttacking = true;

	m_AttackLimitTime = m_AttackLimitTimeMax;

}
