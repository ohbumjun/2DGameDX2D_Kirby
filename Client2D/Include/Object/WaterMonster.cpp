#include "WaterMonster.h"

CWaterMonster::CWaterMonster() 
{
	m_AttackResetTime = 0.f;
	m_AttackResetTimeMax = 2.f;
	m_AttackLimitTime = 0.f;
	m_AttackLimitTimeMax = 4.f;

	m_HP = 200.f;
	m_HPMax = 200.f;
	m_AttackAbility = 40.f;
	m_MonsterType = Monster_Type::Ability;
}

CWaterMonster::CWaterMonster(const CWaterMonster& Monster) : CMonster(Monster)
{}

CWaterMonster::~CWaterMonster()
{}

void CWaterMonster::Start()
{
	CMonster::Start();

	m_HP = 200.f;
	m_HPMax = 200.f;
	m_AttackAbility = 40.f;
	m_MonsterType = Monster_Type::Ability;
}

bool CWaterMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CWaterMonster::UpdateAttackLimitTime(float DeltaTime)
{
	if (m_AttackLimitTime > 0.f)
	{
		m_AttackLimitTime -= DeltaTime;
	}
}

void CWaterMonster::Attack()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	m_IsAttacking = true;

	m_AttackLimitTime = m_AttackLimitTimeMax;
}

void CWaterMonster::ChangeAttackAnimation()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	CMonster::ChangeAttackAnimation();
}

void CWaterMonster::AIDeathSpecific(float DeltaTime)
{
	Destroy();
}

void CWaterMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	UpdateAttackLimitTime(DeltaTime);
}

void CWaterMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CWaterMonster* CWaterMonster::Clone()
{
	return new CWaterMonster(*this);
}
