#include "Kabu.h"

CKabu::CKabu()
{
	SetTypeID<CKabu>();
}

CKabu::~CKabu()
{}

void CKabu::Start()
{
	SetWorldScale(100.f, 100.f, 1.f);

	CNormalMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;
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
