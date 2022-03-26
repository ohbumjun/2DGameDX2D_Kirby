#include "SwordMonster.h"

CSwordMonster::CSwordMonster()
{
	SetTypeID<CSwordMonster>();
}

CSwordMonster::~CSwordMonster()
{}

void CSwordMonster::Start()
{
	CAbilityMonster::Start();
}

bool CSwordMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("SwordMonster", TEXT("Ability_Sword.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CSwordMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CSwordMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}
