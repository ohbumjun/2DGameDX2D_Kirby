#include "WaterMonster.h"

CWaterMonster::CWaterMonster()  :
	m_WaterGoUpTimeMax(3.f),
	m_GoUp(false)
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

	m_HP = 150.f;
	m_HPMax = 150.f;
	m_AttackAbility = 40.f;
	m_MonsterType = Monster_Type::Water;
}

bool CWaterMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CWaterMonster::UpdateWaterGoUpTime(float DeltaTime)
{
	if (m_WaterGoUpTime > 0.f)
	{
		m_WaterGoUpTime -= DeltaTime;
	}
}

void CWaterMonster::CheckBelowWorldResolution()
{
	m_GoUp = true;
	m_WaterGoUpTime = m_WaterGoUpTimeMax;
}

void CWaterMonster::UpdateSwimEffect(float DeltaTime)
{
	if (m_WaterGoUpTime >= 0.f)
	{
		if (m_GoUp)
			AddWorldPos(Vector3(0.f, 1.f, 0.f) * DeltaTime * 90.f);
		else
			AddWorldPos(Vector3(0.f, -1.f, 0.f) * DeltaTime * 30.f);
	}
	else
	{
		// 기존에 올라가고 있었다면, 다시 서서히 내려가게 한다.
		if (m_GoUp)
		{
			m_WaterGoUpTime = (m_WaterGoUpTimeMax * 3 * rand()) + 3;
			m_GoUp = false;
		}
		// 기존에 내려가고 있었다면, 다시 올라가게 한다.
		else
		{
			m_WaterGoUpTime = (m_WaterGoUpTimeMax * 3 * rand()) + 1;
			m_GoUp = true;
		}
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

	UpdateWaterGoUpTime(DeltaTime);
	
	UpdateSwimEffect(DeltaTime);
}

void CWaterMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CWaterMonster* CWaterMonster::Clone()
{
	return new CWaterMonster(*this);
}
