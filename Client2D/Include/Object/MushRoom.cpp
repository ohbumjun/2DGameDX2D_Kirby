#include "MushRoom.h"
#include "../Excel/Excel.h"

CMushRoom::CMushRoom() :
	m_IsToggleGoUp(true),
	m_ToggleLimitTime(0.2f),
	m_ToggleLimitTimeMax(0.2f)
{
	m_JumpVelocity = 30.f;

	SetTypeID<CMushRoom>();
}

CMushRoom::CMushRoom(const CMushRoom& mushRoom) : CNormalMonster(mushRoom)
{
	*this = mushRoom;
}

CMushRoom::~CMushRoom()
{
}

void CMushRoom::AIWalkSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CMushRoom::AITraceSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CMushRoom::UpdateToggle(float DeltaTime)
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


void CMushRoom::Start()
{
	CNormalMonster::Start();

	SetExcelStat(L"MushRoom");

	m_IsGround = true;

	// m_PhysicsSimulate = true;
	// m_IsGroundObject = true;
}

bool CMushRoom::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_MushRoom", TEXT("Normal_MushRoom.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CMushRoom::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CMushRoom::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

CMushRoom* CMushRoom::Clone()
{
	return new CMushRoom(*this);
}
