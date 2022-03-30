#include "BossPenguin.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

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

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Penguin")); //
		}
	}
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
