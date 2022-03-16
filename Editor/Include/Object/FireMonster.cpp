#include "FireMonster.h"

CFireMonster::CFireMonster()
{}

CFireMonster::CFireMonster(const CFireMonster& Monster)
{}

CFireMonster::~CFireMonster()
{}

void CFireMonster::Start()
{
	CAbilityMonster::Start();
}

bool CFireMonster::Init()
{
	if (CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Fire", TEXT("Ability_Fire.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CFireMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CFireMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CFireMonster* CFireMonster::Clone()
{
	return new CFireMonster(*this);
}
