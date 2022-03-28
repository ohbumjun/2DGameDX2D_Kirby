#include "WaterMonster.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CWaterMonster::CWaterMonster()
{}

CWaterMonster::~CWaterMonster()
{}

void CWaterMonster::Start()
{
	CMonster::Start();
}

bool CWaterMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CWaterMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CWaterMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}
