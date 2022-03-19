#include "MiddleBossHammer.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer()
{
	SetTypeID<CMiddleBossHammer>();
}

CMiddleBossHammer::CMiddleBossHammer(const CMiddleBossHammer& Monster) :
	CBossMonster(Monster)
{}

CMiddleBossHammer::~CMiddleBossHammer()
{}

void CMiddleBossHammer::Start()
{
	CBossMonster::Start();
}

bool CMiddleBossHammer::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("MiddleHammer", TEXT("MiddleBoss_Hammer.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(200.f, 200.f, 1.f);

	return true;
}

void CMiddleBossHammer::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);
}

void CMiddleBossHammer::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CMiddleBossHammer* CMiddleBossHammer::Clone()
{
	return new CMiddleBossHammer(*this);
}
