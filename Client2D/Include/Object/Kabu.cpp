#include "Kabu.h"
#include "../Excel/Excel.h"

CKabu::CKabu()
{
	SetTypeID<CKabu>();
}

CKabu::~CKabu()
{}

void CKabu::Start()
{
	SetExcelStat(L"Kabu");

	CNormalMonster::Start();

	// m_PhysicsSimulate = true;
	// SetWorldScale(100.f, 100.f, 1.f);
	// m_IsGroundObject = true;

	m_IsGround = true;

}

bool CKabu::Init()
{
	if (!CNormalMonster::Init())
		return false;

	// LoadAnimationInstance("Normal_Bear", TEXT("Normal_Bear.anim"));

	// SetCurrentAnimation("RightIdle");

	// SetWorldScale(180.f, 180.f, 1.f);

	return true;
}

void CKabu::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);

}

void CKabu::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
