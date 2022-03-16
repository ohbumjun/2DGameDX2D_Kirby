#include "FightMonster.h"

CFightMonster::CFightMonster()
{
	SetTypeID<CFightMonster>();
}

CFightMonster::CFightMonster(const CFightMonster& Monster)
{}

CFightMonster::~CFightMonster()
{}

void CFightMonster::Start()
{
	CAbilityMonster::Start();
}

bool CFightMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Fight", TEXT("Ability_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CFightMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CFightMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CFightMonster* CFightMonster::Clone()
{
	return new CFightMonster(*this);
}
