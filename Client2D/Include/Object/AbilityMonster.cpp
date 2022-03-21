#include "AbilityMonster.h"

CAbilityMonster::CAbilityMonster():
	m_AttackResetTime(0.f),
	m_AttackResetTimeMax(2.f),
	m_AttackLimitTime(0.f),
	m_AttackLimitTimeMax(4.f)
{
	m_AbilityState = Ability_State::End;

	m_HP = 200.f;
	m_HPMax = 200.f;
	m_AttackAbility = 40.f;
	m_MonsterType = Monster_Type::Ability;
}

CAbilityMonster::CAbilityMonster(const CAbilityMonster& Monster) : CMonster(Monster)
{}

CAbilityMonster::~CAbilityMonster()
{}

void CAbilityMonster::Start()
{
	CMonster::Start();

	m_HP = 200.f;
	m_HPMax = 200.f;
	m_AttackAbility = 40.f;
	m_MonsterType = Monster_Type::Ability;
}

bool CAbilityMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CAbilityMonster::UpdateAttackLimitTime(float DeltaTime)
{
	if (m_AttackLimitTime > 0.f)
	{
		m_AttackLimitTime -= DeltaTime;
	}
}

void CAbilityMonster::Attack()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	m_IsAttacking = true;

	m_AttackLimitTime = m_AttackLimitTimeMax;
}

void CAbilityMonster::ChangeAttackAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	CMonster::ChangeAttackAnimation();
}

void CAbilityMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	UpdateAttackLimitTime(DeltaTime);
}

void CAbilityMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CAbilityMonster* CAbilityMonster::Clone()
{
	return new CAbilityMonster(*this);
}
