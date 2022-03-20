#include "BossTree.h"

class CAnimationSequence2DInstance;

CBossTree::CBossTree()
{
	SetTypeID<CBossTree>();
}

CBossTree::CBossTree(const CBossTree& Monster) :
	CBossMonster(Monster)
{}

CBossTree::~CBossTree()
{}

void CBossTree::Start()
{
	CBossMonster::Start();
}

bool CBossTree::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("MiddleHammer", TEXT("MiddleBoss_Hammer.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(200.f, 200.f, 1.f);

	return true;
}

void CBossTree::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);
}

void CBossTree::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CBossTree* CBossTree::Clone()
{
	return new CBossTree(*this);
}
