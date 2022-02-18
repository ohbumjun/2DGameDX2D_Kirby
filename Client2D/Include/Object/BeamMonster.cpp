#include "BeamMonster.h"

CBeamMonster::CBeamMonster()
{}

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
