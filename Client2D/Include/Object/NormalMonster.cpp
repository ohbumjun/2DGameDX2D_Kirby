#include "NormalMonster.h"

CNormalMonster::CNormalMonster()
{
	// SetTypeID<CNormalMonster>();

	m_MonsterType = Monster_Type::Normal;

	m_AttackAbility = 20.f;
	m_HPMax = 100.f;
	m_HP = 100.f;
}

CNormalMonster::CNormalMonster(const CNormalMonster& Monster) : CMonster(Monster)
{}

CNormalMonster::~CNormalMonster()
{}

void CNormalMonster::Start()
{
	CMonster::Start();

	m_MonsterType = Monster_Type::Normal;

	m_AttackAbility = 20.f;
	m_HPMax = 100.f;
	m_HP = 100.f;
}

bool CNormalMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CNormalMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CNormalMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CNormalMonster* CNormalMonster::Clone()
{
	return new CNormalMonster(*this);
}

void CNormalMonster::Save(FILE* pFile)
{
	CMonster::Save(pFile);
}

void CNormalMonster::Load(FILE* pFile)
{
	CMonster::Load(pFile);

}