#include "MiddleBossHammer.h"
#include "HammerGorillaFarAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer() :
	m_IsAttacking(false),
	m_JumpEnable(false),
	m_AttackResetTime(0.f),
	m_AttackResetTimeMax(2.f),
	m_JumpLimitTimeMax(5.f),
	m_CloseAttackDistance(350.f),
	m_FarAttackDistance(700.f)
{
	SetTypeID<CMiddleBossHammer>();
	m_DashDistance = 900.f;
	m_JumpVelocity = 50.f;
	m_AttackDistance = 700.f;
}

CMiddleBossHammer::CMiddleBossHammer(const CMiddleBossHammer& Monster) : CBossMonster(Monster)
{}

CMiddleBossHammer::~CMiddleBossHammer()
{}

void CMiddleBossHammer::Start()
{
	CBossMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

	m_Sprite->GetAnimationInstance()->Play();

	// Close Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CMiddleBossHammer::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CMiddleBossHammer::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CMiddleBossHammer::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CMiddleBossHammer::FarAttack);

	// Hit ���Ŀ���, �ٷ� Idle �� �Ѿ�� �����Ѵ�.
	/*
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
		*/


	m_IsAttacking = false;
}

bool CMiddleBossHammer::Init()
{
	if (!CBossMonster::Init())
		return false;

	// LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	// SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CMiddleBossHammer::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	m_DeathTime += DeltaTime * 1.f;

	if (m_DeathTime >= 30.f)
	{
		m_HP = 0.f;
	}
}

void CMiddleBossHammer::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);

	if (m_AttackResetTime < m_AttackResetTimeMax)
	{
		m_AttackResetTime += DeltaTime;

		if (m_AttackResetTime >= m_AttackResetTimeMax)
		{
			if (m_IsAttacking)
			{
				m_IsAttacking = false;
			}
			m_AttackResetTime -= m_AttackResetTimeMax;
		}
	}
}

CMiddleBossHammer* CMiddleBossHammer::Clone()
{
	return new CMiddleBossHammer(*this);
}

void CMiddleBossHammer::FarAttack()
{
	if (m_IsAttacking)
		return;

	m_IsAttacking = true;

	Vector3 PlayerPos = m_Scene->GetPlayerObject()->GetWorldPos();

	CHammerGorillaFarAttack* AttackEffect = nullptr;

	// ������ ���� �ִٸ� 
	if (m_ObjectMoveDir.x < 0.f)
	{
		// ���
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->SetLeftAttackDir(0.f);
	}
	// ���������� ���� �ִٸ� 
	else
	{
		// ���
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->SetRightAttackDir(0.f);
	}

	AttackEffect->SetFireOwner(this);

	AttackEffect->SetJumpVelocity(80.f);

	AttackEffect->SetPhysicsSimulate(true);

	AttackEffect->JumpStart();

	AttackEffect->SetCreateMultileAfter(true);

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	ChangeIdleAnimation();
}

void CMiddleBossHammer::CloseAttack()
{}

void CMiddleBossHammer::AIAttackSpecific(float DeltaTime)
{
	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer < m_FarAttackDistance)
	{
		ChangeFarAttackAnimation();
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		ChangeCloseAttackAnimation();
	}
}

void CMiddleBossHammer::AITraceSpecific(float DeltaTime)
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

void CMiddleBossHammer::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CMiddleBossHammer::ChangeCloseAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
}

void CMiddleBossHammer::ChangeToIdleAfterHit()
{}

