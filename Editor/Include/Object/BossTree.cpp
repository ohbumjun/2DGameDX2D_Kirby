#include "BossTree.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

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

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Tree")); //
		}
	}
}

bool CBossTree::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("BossTree", TEXT("Boss_Tree.anim"));
	SetCurrentAnimation("LeftIdle");

	SetWorldScale(250.f, 250.f, 1.f);

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
