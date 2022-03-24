#include "MiddleBossScissorBug.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"
#include "HammerGorillaCloseAttack.h"

class CAnimationSequence2DInstance;

CMiddleBossScissorBug::CMiddleBossScissorBug() :
	m_JumpEnable(false),
	m_JumpLimitTimeMax(5.f)
{
	SetTypeID<CMiddleBossScissorBug>();
	m_DashDistance = 1400.f;
	m_JumpVelocity = 60.f;
	m_AttackDistance = 1000.f;
	m_IsGroundObject = true;
	m_CloseAttackDistance = 450.f;
	m_FarAttackDistance = 1000.f;

	m_HP = 3000.f;
	m_HPMax = 3000.f;
}

CMiddleBossScissorBug::~CMiddleBossScissorBug()
{}

void CMiddleBossScissorBug::Start()
{
	CBossMonster::Start();

	m_HP = 3000.f;
	m_HPMax = 3000.f;

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

	m_Sprite->GetAnimationInstance()->Play();

	// Close Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(0.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CMiddleBossScissorBug::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(0.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CMiddleBossScissorBug::CloseAttack);


	// Far Attack
	// - Jump
	// - Fly
	// - 공중에서, RightPrepareGrab
	// - 그대로, AttemptGrab으로 직선 방향 돌진
	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetPlayTime(0.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeFlyAnimation);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetPlayTime(0.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeFlyAnimation);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightPrepareGrab")->SetPlayTime(0.8f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightPrepareGrab")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeAttemptGrabAnimation);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftPrepareGrab")->SetPlayTime(0.8f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftPrepareGrab")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftPrepareGrab")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeAttemptGrabAnimation);


	// Attempt Grab
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttemptGrab")->SetPlayTime(0.7f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttemptGrab")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttemptGrab")->SetEndFunction(this, &CMiddleBossScissorBug::AttemptGrabEndCallback);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttemptGrab")->SetPlayTime(0.7f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttemptGrab")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttemptGrab")->SetEndFunction(this, &CMiddleBossScissorBug::AttemptGrabEndCallback);
	

	m_InitMoveVelocity = m_MonsterMoveVelocity;

	m_InitJumpAccel = m_JumpAccel;

	m_IsAttacking = false;
}

bool CMiddleBossScissorBug::Init()
{
	if (!CBossMonster::Init())
		return false;

	// LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	// SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();


	return true;
}

void CMiddleBossScissorBug::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	UpdateJumpAction(DeltaTime);
}

void CMiddleBossScissorBug::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

void CMiddleBossScissorBug::CloseAttack()
{
	m_IsAttacking = true;

	// Attack Back Effect
	CHammerGorillaCloseAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaCloseAttack>("AttackEffect");
	//
	if (m_ObjectMoveDir.x < 0.f)
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x - GetWorldScale().x * 0.7f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);

		AttackEffect->SetLeftAttackDir();

	}
	else
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x + GetWorldScale().x * 0.7f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);

		AttackEffect->SetRightAttackDir();
	}

	AttackEffect->SetMonsterOwner(this);

	AttackEffect->AddRelativeRotationZ(90.f);

	AttackEffect->SetLifeTime(2.f);

	m_IsAttacking = false;

	ChangeIdleAnimation();
}

void CMiddleBossScissorBug::AIAttackSpecific(float DeltaTime)
{
	if (m_Jump)
		return;

	if (m_IsAttacking)
		return;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer <= m_FarAttackDistance)
	{
		if (m_FarAttackLimitTime > 0.f)
			return;

		m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

		if (m_IsBottomCollided)
		{
			m_Jump = true;
			m_IsGround = false;
			m_FallTime = 0.f;
			m_FallStartY = GetWorldPos().y;
			m_JumpStart = true;
			m_JumpVelocity = 90.f;
			m_JumpLimitTime = m_JumpLimitTimeMax;
			m_MonsterMoveVelocity = 200.f;
			m_JumpAccel = 1.5f;
			ChangeJumpAttackAnimation();
			m_InitTraceDir = m_TraceDir;
		}
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

		ChangeCloseAttackAnimation();
	}
}

void CMiddleBossScissorBug::AITraceSpecific(float DeltaTime)
{
	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	// Player가 왼쪽에 있다는 것
	if (DistToPlayer < 0.f)
		m_ObjectMoveDir.x = -1.f;
	else
		m_ObjectMoveDir.x = 1.f;
}

void CMiddleBossScissorBug::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CMiddleBossScissorBug::ChangeCloseAttackAnimation()
{
	// Player가 왼쪽에 있다는 것
	if (m_Scene->GetPlayerObject()->GetWorldPos().x - GetWorldPos().x < 0.f)
		m_ObjectMoveDir.x = -1.f;
	else
		m_ObjectMoveDir.x = 1.f;

	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
}

void CMiddleBossScissorBug::ChangeFlyAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftFly");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightFly");
}

void CMiddleBossScissorBug::ChangePrepareGrabAnimation()
{

	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftPrepareGrab");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightPrepareGrab");
}

void CMiddleBossScissorBug::GrabActionStart()
{
	m_AttemptGrab = true;

	ChangeAttemptGrabAnimation();

	m_GrabTraceDir = m_Scene->GetPlayerObject()->GetWorldPos() - GetWorldPos();

	m_GrabTraceDir.Normalize();
}

void CMiddleBossScissorBug::ChangeAttemptGrabAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttemptGrab");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttemptGrab");
}

void CMiddleBossScissorBug::AttemptGrabEndCallback()
{
	m_PhysicsSimulate = true;

	m_AttemptGrab = false;

	m_MonsterMoveVelocity = m_InitMoveVelocity;

	ChangeTraceAnimation();
}

void CMiddleBossScissorBug::UpdateGrabAction(float DeltaTime)
{
	if (m_AttemptGrab)
	{
		AddWorldPos(m_GrabTraceDir * DeltaTime * m_MonsterMoveVelocity);
	}
}

void CMiddleBossScissorBug::ChangeJumpAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftJump");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightJump");
}


void CMiddleBossScissorBug::UpdateJumpAction(float DeltaTime)
{
	if (m_Jump && !m_AttemptGrab)
	{
		AddWorldPos(Vector3(m_InitTraceDir.x, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		// 날다가 중간에 떨어지는 시점에 바로 AttemptGrab Animation으로 바꿔준다.
		if (m_PrevJumpDistDiff > 0 && m_JumpDistDiff <= 0)
		{
			ChangePrepareGrabAnimation();

			m_PhysicsSimulate = false;
		}
	}

	if (m_JumpLimitTime >= 0.f)
	{
		m_JumpLimitTime -= DeltaTime;
	}
}

void CMiddleBossScissorBug::SetObjectLand()
{
	CBossMonster::SetObjectLand();

	m_MonsterMoveVelocity = m_InitMoveVelocity;

	m_Jump = false;

	m_AttemptGrab = false;

	m_JumpAccel = m_InitJumpAccel;
}

void CMiddleBossScissorBug::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

