#include "Frog.h"
#include "Component/PaperBurnComponent.h"
#include "UI/UIProgressbar.h"
#include "Component/SpriteComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/PaperBurnComponent.h"
#include "Component/ColliderCircle.h"
#include "MonsterAnimation.h"
#include "Engine.h"
#include "Player2D.h"
#include "../UI/SimpleHUD.h"

CFrog::CFrog() :
	m_IsToggleGoUp(true),
	m_ToggleLimitTime(0.2f),
	m_ToggleLimitTimeMax(0.2f)
{
	m_JumpVelocity = 30.f;

	SetTypeID<CFrog>();
}

CFrog::~CFrog()
{
}

void CFrog::AIWalkSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CFrog::AITraceSpecific(float DeltaTime)
{
	UpdateToggle(DeltaTime);
}

void CFrog::UpdateToggle(float DeltaTime)
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


void CFrog::Start()
{
	CNormalMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;
}

bool CFrog::Init()
{
	if (!CNormalMonster::Init())
		return false;

	LoadAnimationInstance("Normal_MushRoom", TEXT("Normal_MushRoom.anim"));
	SetCurrentAnimation("RightIdle");

	return true;
}

void CFrog::Update(float DeltaTime)
{
	CNormalMonster::Update(DeltaTime);
}

void CFrog::PostUpdate(float DeltaTime)
{
	CNormalMonster::PostUpdate(DeltaTime);
}

