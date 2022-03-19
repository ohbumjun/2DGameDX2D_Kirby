#include "BossMonster.h"

#include <Scene/Scene.h>

#include "Engine.h"

CBossMonster::CBossMonster() :
	m_StartBossStage(false)
{
	m_MonsterType = Monster_Type::Boss;
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

	// World 甘 农扁 力茄
	Resolution RS = CEngine::GetInst()->GetResolution();

	m_InitWorldResolution = m_Scene->GetWorldResolution();

	float WorldRightEnd = GetWorldPos().x + (float)RS.Width * 0.5f;

	m_Scene->SetWorldResolution(WorldRightEnd, m_Scene->GetWorldResolution().y);

	m_StartBossStage = true;
}

void CBossMonster::AIDeathSpecific(float DeltaTime)
{
	m_Scene->SetWorldResolution(m_InitWorldResolution.x, m_InitWorldResolution.y);
}

void CBossMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	// World 甘 农扁 力茄
	Resolution RS = CEngine::GetInst()->GetResolution();

	if (m_Scene->GetPlayerObject()->GetWorldPos().x + (float)RS.Width * 0.5f < GetWorldPos().x)
	{
		MakeBossStartEffect();
	}
}

void CBossMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CBossMonster* CBossMonster::Clone()
{
	return new CBossMonster(*this);
}
