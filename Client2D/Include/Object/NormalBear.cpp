#include "NormalBear.h"
#include "../Excel/Excel.h"

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
	SetExcelStat(L"NormalBear");

	CNormalMonster::Start();

	m_IsGround = true;

	// SetWorldScale(180.f, 180.f, 1.f);
	// m_IsGroundObject = true;
}

bool CNormalBear::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_Bear", TEXT("Normal_Bear.anim"));

	SetCurrentAnimation("RightIdle");

	SetWorldScale(180.f, 180.f, 1.f);

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
