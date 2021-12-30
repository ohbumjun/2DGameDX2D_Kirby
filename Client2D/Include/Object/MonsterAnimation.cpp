#include "MonsterAnimation.h"

CMonsterAnimation::CMonsterAnimation()
{}

CMonsterAnimation::CMonsterAnimation(const CMonsterAnimation& Anim) :
CAnimationSequence2DInstance(Anim)
{}

CMonsterAnimation::~CMonsterAnimation()
{}

bool CMonsterAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init()) //
		return false;

	AddAnimation("PlayerIdle", "Idle");
}

void CMonsterAnimation::Update(float DeltaTime)
{
	CAnimationSequence2DInstance::Update(DeltaTime);
}

void CMonsterAnimation::PostUpdate(float DeltaTime)
{}
