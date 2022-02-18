#include "NormalMonster.h"

CNormalMonster::CNormalMonster()
{}

CNormalMonster::CNormalMonster(const CNormalMonster& Monster)
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

CNormalMonster* CNormalMonster::Clone()
{
	return new CNormalMonster(*this);
}
