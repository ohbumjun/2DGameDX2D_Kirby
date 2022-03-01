#include "AbilityMonster.h"

CAbilityMonster::CAbilityMonster()
{}

CAbilityMonster::CAbilityMonster(const CAbilityMonster& Monster) : CMonster(Monster)
{}

CAbilityMonster::~CAbilityMonster()
{}

void CAbilityMonster::Start()
{
	CMonster::Start();
}

bool CAbilityMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CAbilityMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CAbilityMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CAbilityMonster* CAbilityMonster::Clone()
{
	return new CAbilityMonster(*this);
}
