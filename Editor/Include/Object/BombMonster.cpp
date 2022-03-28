#include "BombMonster.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

class CAnimationSequence2DInstance;

CBombMonster::CBombMonster()
{
	SetTypeID<CBombMonster>();
}

CBombMonster::CBombMonster(const CBombMonster& Monster)
{}

CBombMonster::~CBombMonster()
{}

void CBombMonster::Start()
{
	CAbilityMonster::Start();
}

bool CBombMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("BombMonster", TEXT("Ability_Bomb.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Bomb"));
	}

	return true;
}

void CBombMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CBombMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CBombMonster* CBombMonster::Clone()
{
	return new CBombMonster(*this);
}
