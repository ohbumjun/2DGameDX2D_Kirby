#include "MiddleBossHammer.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer()
{
	SetTypeID<CMiddleBossHammer>();
}

CMiddleBossHammer::CMiddleBossHammer(const CMiddleBossHammer& Monster)
{}

CMiddleBossHammer::~CMiddleBossHammer()
{}

void CMiddleBossHammer::Start()
{
	CMonster::Start();
}

bool CMiddleBossHammer::Init()
{
	if (!CMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CMiddleBossHammer::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CMiddleBossHammer::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CMiddleBossHammer* CMiddleBossHammer::Clone()
{
	return new CMiddleBossHammer(*this);
}
