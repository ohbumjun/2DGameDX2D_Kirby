#include "NormalBear.h"

CNormalBear::CNormalBear()
{}

CNormalBear::~CNormalBear()
{}

void CNormalBear::Start()
{
	CGameObject::Start();
}

bool CNormalBear::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_Bear", TEXT("Normal_Bear.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CNormalBear::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CNormalBear::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNormalBear* CNormalBear::Clone()
{
	return new CNormalBear(*this);
}
