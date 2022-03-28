#include "NormalBear.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CNormalBear::CNormalBear()
{
	SetTypeID<CNormalBear>();
}

CNormalBear::~CNormalBear()
{}

void CNormalBear::Start()
{
	CNormalMonster::Start();
}

bool CNormalBear::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_Bear", TEXT("Normal_Bear.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(200.f, 200.f, 1.f);

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(FindComponentByType<CWidgetComponent>()->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("NormalBear"));
	}

	return true;
}

void CNormalBear::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CNormalBear::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CNormalBear* CNormalBear::Clone()
{
	return new CNormalBear(*this);
}
