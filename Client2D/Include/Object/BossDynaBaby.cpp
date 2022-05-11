#include "BossDynaBaby.h"
#include "../Excel/Excel.h"
#include "BossDyna.h"

CBossDynaBaby::CBossDynaBaby() :
	m_IsToggleGoUp(true),
	m_ToggleLimitTime(0.2f),
	m_ToggleLimitTimeMax(0.2f)
{
	m_JumpVelocity = 30.f;
	m_IsBottomCollided = false;
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

void CBossDynaBaby::AIDeathSpecific(float DeltaTime)
{
	CNormalMonster::AIDeathSpecific(DeltaTime);

	// Boss Dyna 의 Baby List 에 추가 
	m_BossDyna->DeleteDynaBaby(this);
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

void CBossDynaBaby::JumpStart()
{
	m_Jump = true;
	m_IsGround = false;

	m_FallTime = 0.f;
	m_FallStartY = GetWorldPos().y;

	m_JumpStart = true;
}

void CBossDynaBaby::Start()
{
	CNormalMonster::Start();

	SetExcelStat(L"Kabu");

	m_GamePlayDelayTime = -1.f;

	m_IsGround = false;

	m_IsTracingMonster = true;

	m_ApplyLimitPosResolution = false;
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
