#include "BossMonster.h"
#include <Scene/Scene.h>
#include "Engine.h"
#include "Player2D.h"

CBossMonster::CBossMonster() :
	m_StartBossStage(false),
	m_AttackResetTimeMax(2.f),
	m_FarAttackLimitTimeMax(3.5f),
	m_CloseAttackLimitTimeMax(2.5f),
	m_IsRoundStarted(false)
{
	m_MonsterType = Monster_Type::Boss;
	m_AttackAbility = 60.f;
}

CBossMonster::CBossMonster(const CBossMonster& Monster) : CMonster(Monster)
{}

CBossMonster::~CBossMonster()
{}

void CBossMonster::Start()
{
	CMonster::Start();
}

bool CBossMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CBossMonster::MakeBossStartEffect()
{
	if (m_StartBossStage)
		return;

	// 1) World 맵 크기 제한
	m_IsRoundStarted = true;

	Resolution RS = CEngine::GetInst()->GetResolution();

	m_InitWorldResolution = m_Scene->GetWorldResolution();

	float WorldRightEnd = GetWorldPos().x + (float)RS.Width * 0.5f;

	if (m_Scene->GetWorldResolution().x <= WorldRightEnd)
	{
		WorldRightEnd = m_Scene->GetWorldResolution().x;
	}

	m_Scene->SetWorldResolution(WorldRightEnd, m_Scene->GetWorldResolution().y);

	m_StartBossStage = true;

	// 2) 카메라 이동 효과
	CPlayer2D* Player = dynamic_cast<CPlayer2D*>(m_Scene->GetPlayerObject());

	if (!Player)
		return;

	Player->SetCameraFollowBossMonster(this);
}

void CBossMonster::UpdateAttackResetTime(float DeltaTime)
{
	if (m_AttackResetTime < m_AttackResetTimeMax)
	{
		m_AttackResetTime += DeltaTime;
	}
	else
	{
		if (m_IsAttacking)
		{
			m_IsAttacking = false;
		}
		m_AttackResetTime -= m_AttackResetTimeMax;
	}
}

void CBossMonster::UpdateAttackLimitTimes(float DeltaTime)
{
	if (m_FarAttackLimitTime > 0.f)
	{
		m_FarAttackLimitTime -= DeltaTime;
	}

	if (m_CloseAttackLimitTime > 0.f)
	{
		m_CloseAttackLimitTime -= DeltaTime;
	}
}

void CBossMonster::AIDeathSpecific(float DeltaTime)
{
	m_Scene->SetWorldResolution(m_InitWorldResolution.x, m_InitWorldResolution.y);
}

void CBossMonster::Update(float DeltaTime)
{
	if (m_GamePlayDelayTime > 0)
	{
		m_GamePlayDelayTime -= DeltaTime;
		return;
	}

	// World 맵 크기 제한
	Resolution RS = CEngine::GetInst()->GetResolution();

	if (m_Scene->GetPlayerObject()->GetWorldPos().x + (float)RS.Width * 0.5f < GetWorldPos().x)
	{
		MakeBossStartEffect();
	}

	if (!m_IsRoundStarted)
		return;

	CMonster::Update(DeltaTime);

	UpdateAttackResetTime(DeltaTime);

	UpdateAttackLimitTimes(DeltaTime);
}

void CBossMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CBossMonster* CBossMonster::Clone()
{
	return new CBossMonster(*this);
}
