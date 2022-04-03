#include "BossDynaBaby.h"
#include "../Excel/Excel.h"

CBossDynaBaby::CBossDynaBaby() :
	m_IsToggleGoUp(true),
	m_ToggleLimitTime(0.2f),
	m_ToggleLimitTimeMax(0.2f)
{
	m_JumpVelocity = 30.f;

	SetTypeID<CBossDynaBaby>();
}

CBossDynaBaby::~CBossDynaBaby()
{
}

void CBossDynaBaby::AIWalkSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CBossDynaBaby::AITraceSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CBossDynaBaby::UpdateToggle(float DeltaTime)
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

void CBossDynaBaby::Start()
{
	CNormalMonster::Start();

	SetExcelStat(L"MushRoom");

	m_IsGround = true;

	// m_PhysicsSimulate = true;
	// m_IsGroundObject = true;
}

bool CBossDynaBaby::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("DynaBaby", TEXT("Boss_Dyna_Baby.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CBossDynaBaby::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CBossDynaBaby::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}
