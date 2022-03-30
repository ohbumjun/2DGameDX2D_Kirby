#include "Chicken.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CChicken::CChicken()
{
	SetTypeID<CChicken>();
}

CChicken::~CChicken()
{}

void CChicken::Start()
{
	CNormalMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Chicken")); //
		}
	}

}

bool CChicken::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Chicken", TEXT("Normal_Chicken.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Chicken"));
	}


	return true;
}

void CChicken::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CChicken::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
