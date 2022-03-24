#include "MiddleBossScissorBug.h"

class CAnimationSequence2DInstance;

CMiddleBossScissorBug::CMiddleBossScissorBug()
{
	SetTypeID<CMiddleBossScissorBug>();
}

CMiddleBossScissorBug::~CMiddleBossScissorBug()
{}

void CMiddleBossScissorBug::Start()
{
	CBossMonster::Start();
}

bool CMiddleBossScissorBug::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("MiddleScissorBug", TEXT("MiddleBoss_ScissorBug.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(250.f, 250.f, 1.f);

	return true;
}

void CMiddleBossScissorBug::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);
}

void CMiddleBossScissorBug::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}
