#include "Squid.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CSquid::CSquid()
{
	SetTypeID<CSquid>();
}

CSquid::CSquid(const CSquid& Monster)
{}

CSquid::~CSquid()
{}

void CSquid::Start()
{
	CWaterMonster::Start();
}

bool CSquid::Init()
{
	if (!CWaterMonster::Init())
		return false;

	LoadAnimationInstance("Squid", TEXT("Normal_Squid.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(m_SimpleHUDWidget->GetWidgetWindow());

	if (MonsterHUD)
	{
		MonsterHUD->SetText(TEXT("Squid"));
	}

	return true;
}

void CSquid::Update(float DeltaTime)
{
	CWaterMonster::Update(DeltaTime);
}

void CSquid::PostUpdate(float DeltaTime)
{
	CWaterMonster::PostUpdate(DeltaTime);
}

CSquid* CSquid::Clone()
{
	return new CSquid(*this);
}
