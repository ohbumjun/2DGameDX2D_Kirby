#include "Pig.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CPig::CPig()
{
	SetTypeID<CPig>();
}

CPig::~CPig()
{}

void CPig::Start()
{
	CNormalMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Pig")); //
		}
	}

}

bool CPig::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Pig", TEXT("Normal_Pig.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Pig"));
	}


	return true;
}

void CPig::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CPig::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
