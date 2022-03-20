#include "MiddleBossHammer.h"
#include "HammerGorillaFarAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"
#include "HammerGorillaCloseAttack.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer() :
	m_IsAttacking(false),
	m_JumpEnable(false),
	m_AttackResetTime(0.f),
	m_AttackResetTimeMax(2.f),
	m_JumpLimitTimeMax(8.f),
	m_CloseAttackDistance(250.f),
	m_FarAttackDistance(500.f),
	m_JumpDistance(1000.f),
	m_DashRunDistance(800.f)
{
	SetTypeID<CMiddleBossHammer>();
	m_DashDistance = 1000.f;
	m_JumpVelocity = 50.f;
	m_AttackDistance = 500.f;
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

	// Hit 이후에는, 바로 Idle 로 넘어가게 세팅한다.
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

	if (m_JumpLimitTime >= 0.f)
	{
		m_JumpLimitTime -= DeltaTime;
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

	// 왼쪽을 보고 있다면 
	if (m_ObjectMoveDir.x < 0.f)
	{
		// 가운데
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->SetLeftAttackDir(0.f);
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		// 가운데
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

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	ChangeIdleAnimation();
}

void CMiddleBossHammer::CloseAttack()
{
	// Attack Back Effect
	CHammerGorillaCloseAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaCloseAttack>("AttackEffect");
	//
	if (m_ObjectMoveDir.x < 0.f)
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x - GetWorldScale().x * 0.3f,
				GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);
	}
	else
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x + GetWorldScale().x * 0.3f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);
	}

	AttackEffect->AddRelativeRotationZ(90.f);

	AttackEffect->SetLifeTime(0.5f);

	AttackEffect->SetAttackDirX(0.f);
}

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
	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_DashRunDistance && DistToPlayer <= m_JumpDistance)
	{
		if (m_IsBottomCollided && m_JumpLimitTime <= 0.f)
		{
			m_Jump = true;
			m_IsGround = false;
			m_FallTime = 0.f;
			m_FallStartY = GetWorldPos().y;
			m_JumpStart = true;
			m_JumpVelocity = 130.f;
			m_JumpLimitTime = m_JumpLimitTimeMax;
			m_MonsterMoveVelocity = 300.f;
			ChangeJumpAttackAnimation();
		}
	}
	else if (DistToPlayer < m_DashRunDistance)
	{
		// 그냥 계속 그대로 진행 (ChangeTraceAnimation)
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

void CMiddleBossHammer::ChangeJumpAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftJump");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightJump");
}

void CMiddleBossHammer::ChangeJumpEndAttackAnimation()
{}

void CMiddleBossHammer::ChangeToIdleAfterHit()
{}

void CMiddleBossHammer::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

