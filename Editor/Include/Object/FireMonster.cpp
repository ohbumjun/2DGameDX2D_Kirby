#include "FireMonster.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CFireMonster::CFireMonster()
{
	SetTypeID<CFireMonster>();
}

CFireMonster::CFireMonster(const CFireMonster& Monster)
{}

CFireMonster::~CFireMonster()
{}

void CFireMonster::Start()
{
	CAbilityMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Fire")); //
		}
	}
}

bool CFireMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Fire", TEXT("Ability_Fire.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Fire"));
	}

	return true;
}

void CFireMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CFireMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CFireMonster* CFireMonster::Clone()
{
	return new CFireMonster(*this);
}
