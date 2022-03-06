#include "NormalBear.h"

CNormalBear::CNormalBear()
{
	SetTypeID<CNormalBear>();
}

CNormalBear::CNormalBear(const CNormalBear& Beatle) : CNormalMonster(Beatle)
{}

CNormalBear::~CNormalBear()
{}

void CNormalBear::Start()
{
	CNormalMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;
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
	CNormalMonster::Update(DeltaTime);

}

void CNormalBear::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CNormalBear* CNormalBear::Clone()
{
	return new CNormalBear(*this);
}
