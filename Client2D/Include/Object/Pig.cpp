#include "Pig.h"
#include "Component/WidgetComponent.h"
#include "Engine.h"
#include "Player2D.h"
#include "../Excel/Excel.h"

CPig::CPig() :
	m_IsToggleGoUp(true),
	m_ToggleLimitTime(0.2f),
	m_ToggleLimitTimeMax(0.2f)
{
	m_JumpVelocity = 30.f;

	SetTypeID<CPig>();
}

CPig::~CPig()
{
}

void CPig::AIWalkSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CPig::AITraceSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CPig::UpdateToggle(float DeltaTime)
{
	if (m_IsBottomCollided)
	{
		m_Jump = true;
		m_IsGround = false;

		m_FallTime = 0.f;
		m_FallStartY = GetWorldPos().y;

		m_JumpStart = true;
	}
}


void CPig::Start()
{
	CNormalMonster::Start();

	Monster_Stat* Stat = CExcel::GetInst()->FindMonsterState(L"Pig");

	m_MonsterType = Stat->m_Type;
	m_MonsterMoveVelocity = Stat->m_MoveVelocity;
	m_AttackDistance = Stat->m_AttackDist;
	m_DashDistance = Stat->m_DashDist;
	m_HPMax = Stat->m_HP;
	m_HP = Stat->m_HP;
	m_IsGroundObject = Stat->m_IsGroundObject;

	if (!m_IsGroundObject)
		m_PhysicsSimulate = false;
	else
		m_PhysicsSimulate = true;

	m_AttackAbility = Stat->m_AttackAbility;
	SetWorldScale(Stat->m_Scale, Stat->m_Scale, 1.f);
	m_JumpVelocity = Stat->m_JumpVelocity;

	// m_PhysicsSimulate = true;

	m_IsGround = true;

	// m_IsGroundObject = true;
}

bool CPig::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_MushRoom", TEXT("Normal_MushRoom.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CPig::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CPig::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
