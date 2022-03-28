#include "PurpleBeatle.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CPurpleBeatle::CPurpleBeatle()
{
	SetTypeID<CPurpleBeatle>();
}

CPurpleBeatle::~CPurpleBeatle()
{}

void CPurpleBeatle::Start()
{
	CGameObject::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Beatle")); //
		}
	}
}

bool CPurpleBeatle::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_PurpleBeatle", TEXT("Normal_PurpleBeatle.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Beatle"));
	}

	return true;
}

void CPurpleBeatle::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CPurpleBeatle::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CPurpleBeatle* CPurpleBeatle::Clone()
{
	return new CPurpleBeatle(*this);
}
