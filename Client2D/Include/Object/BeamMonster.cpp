#include "BeamMonster.h"

class CAnimationSequence2DInstance;

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

	LoadAnimationInstance("Beam", TEXT("Kirby_Fight.anim"));
	SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

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
