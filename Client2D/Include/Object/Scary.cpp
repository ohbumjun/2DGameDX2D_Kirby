#include "Scary.h"
#include "../Excel/Excel.h"

CScary::CScary() :
	m_IsToggleGoUp(true),
	m_ToggleLimitTime(0.2f),
	m_ToggleLimitTimeMax(0.2f)
{
	m_JumpVelocity = 30.f;

	SetTypeID<CScary>();
}


CScary::~CScary()
{
}

void CScary::AIWalkSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CScary::AITraceSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CScary::UpdateToggle(float DeltaTime)
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


void CScary::Start()
{
	CNormalMonster::Start();

	SetExcelStat(L"Scary");

	m_IsGround = true;

	// m_PhysicsSimulate = true;
	// m_IsGroundObject = true;
}

bool CScary::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_Scary", TEXT("Normal_Scary.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CScary::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CScary::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

