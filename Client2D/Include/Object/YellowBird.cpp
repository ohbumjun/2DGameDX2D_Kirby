#include "YellowBird.h"

CYellowBird::CYellowBird()
{}

CYellowBird::CYellowBird(const CYellowBird& Bird) : CNormalMonster(Bird)
{}

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
