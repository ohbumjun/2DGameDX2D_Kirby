#include "YellowBird.h"

CYellowBird::CYellowBird()
{
	SetTypeID<CYellowBird>();
}

CYellowBird::CYellowBird(const CYellowBird& Bird) : CNormalMonster(Bird)
{}

CYellowBird::~CYellowBird()
{}

void CYellowBird::Start()
{
	CNormalMonster::Start();
}

bool CYellowBird::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_YellowBird", TEXT("Normal_YellowBird.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CYellowBird::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CYellowBird::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CYellowBird* CYellowBird::Clone()
{
	return new CYellowBird(*this);
}
