#include "NormalBear.h"

CNormalBear::CNormalBear()
{
	SetTypeID<CNormalBear>();
}

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

	SetWorldScale(200.f, 200.f, 1.f);

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
