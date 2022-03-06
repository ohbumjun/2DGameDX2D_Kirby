#include "PurpleBeatle.h"

CPurpleBeatle::CPurpleBeatle()
{
	SetTypeID<CPurpleBeatle>();
	m_PhysicsSimulate = false;
}

CPurpleBeatle::CPurpleBeatle(const CPurpleBeatle& Beatle) : CNormalMonster(Beatle)
{}

CPurpleBeatle::~CPurpleBeatle()
{}

void CPurpleBeatle::Start()
{
	CNormalMonster::Start();
}

bool CPurpleBeatle::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_PurpleBeatle", TEXT("Normal_PurpleBeatle.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CPurpleBeatle::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CPurpleBeatle::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CPurpleBeatle* CPurpleBeatle::Clone()
{
	return new CPurpleBeatle(*this);
}
