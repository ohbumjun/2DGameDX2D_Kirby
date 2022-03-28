#include "NormalMonster.h"
#include "Component/WidgetComponent.h"
#include "UI/MonsterEditorHUD.h"

CNormalMonster::CNormalMonster()
{}

CNormalMonster::CNormalMonster(const CNormalMonster& Monster) : CMonster(Monster)
{}

CNormalMonster::~CNormalMonster()
{}

void CNormalMonster::Start()
{
	CMonster::Start();
}

bool CNormalMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CNormalMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CNormalMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CNormalMonster::Save(FILE* pFile)
{
	CMonster::Save(pFile);
}

void CNormalMonster::Load(FILE* pFile)
{
	CMonster::Load(pFile);

}