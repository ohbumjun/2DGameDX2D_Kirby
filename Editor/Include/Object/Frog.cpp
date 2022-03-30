#include "Frog.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CFrog::CFrog()
{
	SetTypeID<CFrog>();
}

CFrog::~CFrog()
{}

void CFrog::Start()
{
	CNormalMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Frog")); //
		}
	}

}

bool CFrog::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Frog", TEXT("Normal_Frog.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Pig"));
	}


	return true;
}

void CFrog::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CFrog::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
