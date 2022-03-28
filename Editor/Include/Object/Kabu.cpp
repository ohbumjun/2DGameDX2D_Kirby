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
}

bool CKabu::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Kabu", TEXT("Normal_Kabu.anim"));
	// LoadAnimationInstance("Normal_YellowBird", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");


	CMonsterEditorHUD* MonsterHUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();

	MonsterHUD->SetText(TEXT("Kabu"));
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
