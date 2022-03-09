#include "BeamMonster.h"

class CAnimationSequence2DInstance;

CBeamMonster::CBeamMonster()
{
	SetTypeID<CBeamMonster>();

	m_AbilityState = Ability_State::Beam;
}

CBeamMonster::CBeamMonster(const CBeamMonster& Monster) : CAbilityMonster(Monster)
{}

CBeamMonster::~CBeamMonster()
{}

void CBeamMonster::Start()
{
	CAbilityMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;
}

bool CBeamMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

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
