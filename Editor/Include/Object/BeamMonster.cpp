#include "BeamMonster.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

class CAnimationSequence2DInstance;

CBeamMonster::CBeamMonster()
{
	SetTypeID<CBeamMonster>();
}

CBeamMonster::CBeamMonster(const CBeamMonster& Monster)
{}

CBeamMonster::~CBeamMonster()
{}

void CBeamMonster::Start()
{
	CAbilityMonster::Start();
}

bool CBeamMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));
	SetCurrentAnimation("RightIdle");

	CMonsterEditorHUD* MonsterHUD = (CMonsterEditorHUD*)m_SimpleHUDWidget->GetWidgetWindow();
	MonsterHUD->SetText(TEXT("Beam"));

	return true;
}

void CBeamMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CBeamMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CBeamMonster* CBeamMonster::Clone()
{
	return new CBeamMonster(*this);
}
