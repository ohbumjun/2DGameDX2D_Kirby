#include "MiddleBossScissorBug.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"

class CAnimationSequence2DInstance;

CMiddleBossScissorBug::CMiddleBossScissorBug()
{
	SetTypeID<CMiddleBossScissorBug>();
}

CMiddleBossScissorBug::~CMiddleBossScissorBug()
{}

void CMiddleBossScissorBug::Start()
{
	CBossMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("ScissorBug")); //
		}
	}
}

bool CMiddleBossScissorBug::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("MiddleScissorBug", TEXT("MiddleBoss_ScissorBug.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(250.f, 250.f, 1.f);

	return true;
}

void CMiddleBossScissorBug::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);
}

void CMiddleBossScissorBug::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}
