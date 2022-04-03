#include "Scary.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CScary::CScary()
{
	SetTypeID<CScary>();
}

CScary::~CScary()
{}

void CScary::Start()
{
	CNormalMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Scary")); //
		}
	}
}

bool CScary::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Scary", TEXT("Normal_Scary.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(200.f, 200.f, 1.f);

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Scary"));
	}

	return true;
}

void CScary::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CScary::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
