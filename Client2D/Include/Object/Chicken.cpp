#include "Chicken.h"
#include "../Excel/Excel.h"

CChicken::CChicken()
{
	m_PhysicsSimulate = false;

	SetTypeID<CChicken>();
}

CChicken::~CChicken()
{}

void CChicken::Start()
{
	CNormalMonster::Start();

	SetExcelStat(L"Chicken");

	// m_PhysicsSimulate = false;
}

bool CChicken::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_YellowBird", TEXT("Normal_YellowBird.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CChicken::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CChicken::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
