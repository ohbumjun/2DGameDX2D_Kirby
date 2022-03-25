#include "BossPenguin.h"

class CAnimationSequence2DInstance;

CBossPenguin::CBossPenguin()
{
	SetTypeID<CBossPenguin>();
}

CBossPenguin::CBossPenguin(const CBossPenguin& Monster) :
	CBossMonster(Monster)
{}

CBossPenguin::~CBossPenguin()
{}

void CBossPenguin::Start()
{
	CBossMonster::Start();
}

bool CBossPenguin::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("BossPenguin", TEXT("Boss_Penguin.anim"));
	SetCurrentAnimation("LeftIdle");

	SetWorldScale(300.f, 300.f, 1.f);

	return true;
}

void CBossPenguin::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);
}

void CBossPenguin::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CBossPenguin* CBossPenguin::Clone()
{
	return new CBossPenguin(*this);
}
