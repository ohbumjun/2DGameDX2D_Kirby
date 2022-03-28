#include "YellowBird.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CYellowBird::CYellowBird()
{
	SetTypeID<CYellowBird>();
}

CYellowBird::~CYellowBird()
{}

void CYellowBird::Start()
{
	CNormalMonster::Start();
}

bool CYellowBird::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_YellowBird", TEXT("Normal_YellowBird.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	MonsterHUD->SetText(TEXT("Bird"));

	return true;
}

void CYellowBird::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CYellowBird::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CYellowBird* CYellowBird::Clone()
{
	return new CYellowBird(*this);
}
