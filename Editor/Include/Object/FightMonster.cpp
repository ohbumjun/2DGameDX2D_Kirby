#include "FightMonster.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CFightMonster::CFightMonster()
{
	SetTypeID<CFightMonster>();
}

CFightMonster::CFightMonster(const CFightMonster& Monster)
{}

CFightMonster::~CFightMonster()
{}

void CFightMonster::Start()
{
	CAbilityMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Fight")); //
		}
	}
}

bool CFightMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Fight", TEXT("Ability_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Fight"));
	}

	return true;
}

void CFightMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CFightMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CFightMonster* CFightMonster::Clone()
{
	return new CFightMonster(*this);
}
