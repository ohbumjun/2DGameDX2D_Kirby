#include "Kabu.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CKabu::CKabu()
{
	SetTypeID<CKabu>();
}

CKabu::~CKabu()
{}

void CKabu::Start()
{
	CNormalMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Kabu")); //
		}
	}

}

bool CKabu::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Kabu", TEXT("Normal_Kabu.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Kabu"));
	}


	return true;
}

void CKabu::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CKabu::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
