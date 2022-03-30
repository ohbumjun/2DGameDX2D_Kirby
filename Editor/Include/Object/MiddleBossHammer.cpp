#include "MiddleBossHammer.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer()
{
	SetTypeID<CMiddleBossHammer>();
}

CMiddleBossHammer::~CMiddleBossHammer()
{}

void CMiddleBossHammer::Start()
{
	CBossMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Hammer")); //
		}
	}
}

bool CMiddleBossHammer::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("MiddleHammer", TEXT("MiddleBoss_Hammer.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(200.f, 200.f, 1.f);

	return true;
}

void CMiddleBossHammer::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);
}

void CMiddleBossHammer::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}
