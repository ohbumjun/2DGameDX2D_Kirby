#include "Fish.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CFish::CFish()
{
	SetTypeID<CFish>();
}

CFish::~CFish()
{}

void CFish::Start()
{
	CWaterMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Fish")); //
		}
	}
}

bool CFish::Init()
{
	if (!CWaterMonster::Init())
		return false;

	LoadAnimationInstance("Fish", TEXT("Normal_Fish.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(m_SimpleHUDWidget->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Squid"));
	}

	return true;
}

void CFish::Update(float DeltaTime)
{
	CWaterMonster::Update(DeltaTime);
}

void CFish::PostUpdate(float DeltaTime)
{
	CWaterMonster::PostUpdate(DeltaTime);
}

