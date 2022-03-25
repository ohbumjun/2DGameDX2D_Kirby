#include "MiddleBossScissorBug.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"
#include "HammerGorillaCloseAttack.h"

class CAnimationSequence2DInstance;

CMiddleBossScissorBug::CMiddleBossScissorBug() :
	m_GrabLimitTimeMax(1.5f),
	m_AttemptGrab(false),
	m_GrabLimitTime(-1.f)
{
	SetTypeID<CMiddleBossScissorBug>();
	m_DashDistance = 1400.f;
	m_JumpVelocity = 60.f;
	m_AttackDistance = 1005.f;
	m_IsGroundObject = true;
	m_CloseAttackDistance = 450.f;
	m_FarAttackDistance = 1000.f;
	m_FarAttackLimitTimeMax = 4.f;

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
	// - ���߿���, RightPrepareGrab
	// - �״��, AttemptGrab���� ���� ���� ����
	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetPlayTime(0.2f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeFlyAnimation);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetPlayTime(0.2f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeFlyAnimation);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightPrepareGrab")->SetPlayTime(1.2f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightPrepareGrab")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightPrepareGrab")->SetEndFunction(this, &CMiddleBossScissorBug::GrabActionStart);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftPrepareGrab")->SetPlayTime(1.2f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftPrepareGrab")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftPrepareGrab")->SetEndFunction(this, &CMiddleBossScissorBug::GrabActionStart);

	// Attempt  Grab
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttemptGrab")->SetPlayTime(m_GrabLimitTimeMax);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttemptGrab")->SetPlayTime(m_GrabLimitTimeMax);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttemptGrab")->SetLoop(true);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttemptGrab")->SetLoop(true);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttemptGrab")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeAnimationAfterGrab);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttemptGrab")->SetEndFunction(this, &CMiddleBossScissorBug::ChangeAnimationAfterGrab);
	
	m_InitMoveVelocity = m_MonsterMoveVelocity;

	m_InitJumpAccel = m_JumpAccel;

	m_IsAttacking = false;

	ChangeIdleAnimation();
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

	UpdateGrabAction(DeltaTime);
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

	AttackEffect->SetEffectMoveSpeed(550.f);

	m_IsAttacking = false;

	ChangeIdleAnimation();
}

void CMiddleBossScissorBug::AIAttackSpecific(float DeltaTime)
{

	if (m_Jump)
		return;

	if (m_IsAttacking)
		return;

	if (m_AttemptGrab)
		return;

	// Player�� ���ʿ� �ִٴ� ��
	if (m_Scene->GetPlayerObject()->GetWorldPos().x - GetWorldPos().x < 0.f)
		m_ObjectMoveDir.x = -1.f;
	else
		m_ObjectMoveDir.x = 1.f;

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
			m_JumpVelocity = 110.f;
			m_JumpAccel = 1.5f;
			ChangeJumpAttackAnimation();
			m_InitTraceDir = m_TraceDir;
			m_InitMoveVelocity = m_MonsterMoveVelocity;
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

	// Player�� ���ʿ� �ִٴ� ��
	if (DistToPlayer < 0.f)
		m_ObjectMoveDir.x = -1.f;
	else
		m_ObjectMoveDir.x = 1.f;
}

void CMiddleBossScissorBug::ChangeCloseAttackAnimation()
{
	// Ȥ�ó� ���߿� ���ö��µ�, �� ���̿� Close Attack�� �ϰ� �Ǵ� ���
	if (!m_PhysicsSimulate)
	{
		EnablePhysics();
	}

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
	m_MonsterMoveVelocity = 600.f;

	m_AttemptGrab = true;

	m_GrabLimitTime = m_GrabLimitTimeMax;

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
	m_IsAttacking = false;

	m_AttemptGrab = false;

	EnablePhysics();
}

void CMiddleBossScissorBug::UpdateGrabAction(float DeltaTime)
{
	if (m_AttemptGrab && m_GrabLimitTime >= 0.f)
	{
		m_IsAttacking = true;

		m_GrabLimitTime -= DeltaTime;

		AddWorldPos(Vector3(m_GrabTraceDir.x, m_GrabTraceDir.y, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (m_GrabLimitTime < 0.01f)
		{
			AttemptGrabEndCallback();
		}
	}
}

void CMiddleBossScissorBug::ChangeAnimationAfterGrab()
{
	if (!m_IsBottomCollided)
	{
		ChangeFlyAnimation();
	}
	else
	{
		ChangeIdleAnimation();
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

		// ���ٰ� �߰��� �������� ������ �ٷ� AttemptGrab Animation���� �ٲ��ش�.
		if (m_PrevPos.y > GetWorldPos().y)
		{
			ChangePrepareGrabAnimation();

			m_PhysicsSimulate = false;

			m_Jump = false;
		}
	}
}

void CMiddleBossScissorBug::EnablePhysics()
{
	m_PhysicsSimulate = true;

	m_MonsterMoveVelocity = m_InitMoveVelocity;

	m_FallTime = 0.f;

	m_FallStartY = GetWorldPos().y;
}

void CMiddleBossScissorBug::SetObjectLand()
{
	CBossMonster::SetObjectLand();

	m_MonsterMoveVelocity = m_InitMoveVelocity;

	m_Jump = false;

	m_PhysicsSimulate = true;

	m_JumpAccel = m_InitJumpAccel;
}

void CMiddleBossScissorBug::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	// if (m_AttemptGrab)
	//	return;

	CBossMonster::ChangeTraceAnimation();
}
