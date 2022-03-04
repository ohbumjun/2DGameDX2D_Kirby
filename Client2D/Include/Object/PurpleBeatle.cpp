#include "PurpleBeatle.h"

CPurpleBeatle::CPurpleBeatle()
{
	SetTypeID<CPurpleBeatle>();
}

CPurpleBeatle::CPurpleBeatle(const CPurpleBeatle& Beatle) : CNormalMonster(Beatle)
{}

CPurpleBeatle::~CPurpleBeatle()
{}

void CPurpleBeatle::Start()
{
	CGameObject::Start();
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
	CGameObject::Update(DeltaTime);
}

void CPurpleBeatle::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPurpleBeatle* CPurpleBeatle::Clone()
{
	return new CPurpleBeatle(*this);
}
