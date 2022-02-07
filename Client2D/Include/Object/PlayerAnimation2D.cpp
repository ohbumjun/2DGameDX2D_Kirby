#include "PlayerAnimation2D.h"

CPlayerAnimation2D::CPlayerAnimation2D()
{
}

CPlayerAnimation2D::CPlayerAnimation2D(const CPlayerAnimation2D& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CPlayerAnimation2D::~CPlayerAnimation2D()
{
}

bool CPlayerAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	// AddAnimation("PlayerIdle", "Idle");
	AddAnimation(TEXT("Kirby_RightRun.sqc"), ENGINE_SEQUENCE_PATH,
		"Idle", true);
	return true;
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}
