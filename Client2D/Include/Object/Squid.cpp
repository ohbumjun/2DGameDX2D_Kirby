#include "Squid.h"
#include "BeamMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "../Excel/Excel.h"

#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CSquid::CSquid()
{
	SetTypeID<CSquid>();
}

CSquid::CSquid(const CSquid& Monster) : CWaterMonster(Monster)
{}

CSquid::~CSquid()
{
}

void CSquid::Start()
{
	CWaterMonster::Start();

	Monster_Stat* Stat = CExcel::GetInst()->FindMonsterState(L"Squid");

	m_MonsterType = Stat->m_Type;
	m_MonsterMoveVelocity = Stat->m_MoveVelocity;
	m_AttackDistance = Stat->m_AttackDist;
	m_DashDistance = Stat->m_DashDist;
	m_HPMax = Stat->m_HP;
	m_HP = Stat->m_HP;
	m_IsGroundObject = Stat->m_IsGroundObject;
	if (!m_IsGroundObject)
	{
		m_PhysicsSimulate = false;
	}
	m_AttackAbility = Stat->m_AttackAbility;
	SetWorldScale(Stat->m_Scale, Stat->m_Scale, 1.f);
	m_JumpVelocity = Stat->m_JumpVelocity;
	
	m_IsGround = true;

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_IsAttacking = false;
}

bool CSquid::Init()
{
	if (!CWaterMonster::Init())
		return false;

	// LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	// SetCurrentAnimation("RightIdle");

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

void CSquid::ChangeIdleAnimation()
{
	CMonster::ChangeTraceAnimation();
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
