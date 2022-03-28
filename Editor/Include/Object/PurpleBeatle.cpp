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
}

bool CPurpleBeatle::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_PurpleBeatle", TEXT("Normal_PurpleBeatle.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	MonsterHUD->SetText(TEXT("Beatle"));

	return true;
}

void CPurpleBeatle::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPurpleBeatle::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPurpleBeatle* CPurpleBeatle::Clone()
{
	return new CPurpleBeatle(*this);
}
