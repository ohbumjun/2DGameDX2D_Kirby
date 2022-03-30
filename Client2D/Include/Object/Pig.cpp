#include "Pig.h"
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

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;
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
