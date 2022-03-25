#include "BossPenguin.h"
#include "HammerGorillaFarAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"
#include "PenguinCloseAttack.h"

class CAnimationSequence2DInstance;

CBossPenguin::CBossPenguin() :
	m_JumpEnable(false),
	m_JumpLimitTimeMax(5.f),
	m_JumpDistance(1200.f),
	m_DashRunDistance(1000.f),
	m_FarAttackLimitTimeMax(2.5f),
	m_FarAttackLimitTime(-1.f)
{
	SetTypeID<CBossPenguin>();
	m_DashDistance = 1200.f;
	m_JumpVelocity = 60.f;
	m_AttackDistance = 950.f;
	m_IsGroundObject = true;
	m_CloseAttackDistance = 400.f;
	m_FarAttackDistance = 950.f;

	m_HP = 5000.f;
	m_HPMax = 5000.f;
}

CBossPenguin::~CBossPenguin()
{}

void CBossPenguin::Start()
{
	CBossMonster::Start();

	m_HP = 5000.f;
	m_HPMax = 5000.f;

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

	m_Sprite->GetAnimationInstance()->Play();

	// Close Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(0.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CBossPenguin::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(0.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CBossPenguin::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(1.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CBossPenguin::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(1.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CBossPenguin::FarAttack);

	// Jump
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetPlayTime(1.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetPlayTime(1.5f);

	m_InitMoveVelocity = m_MonsterMoveVelocity;

	m_InitJumpAccel = m_JumpAccel;

	m_IsAttacking = false;
}

bool CBossPenguin::Init()
{
	if (!CBossMonster::Init())
		return false;

	return true;
}

void CBossPenguin::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	UpdateJumpAction(DeltaTime);

	UpdateFarAttackAction(DeltaTime);
}

void CBossPenguin::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

void CBossPenguin::FarAttack()
{
	m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

	m_MonsterMoveVelocity = 700.f;

	m_IsAttacking = true;

	m_FarAttackTraceDir = m_Scene->GetPlayerObject()->GetWorldPos() - GetWorldPos();

	m_FarAttackTraceDir.Normalize();
}

void CBossPenguin::CloseAttack()
{
	m_IsAttacking = true;

	// Attack Back Effect
	CPenguinCloseAttack* AttackEffect = m_Scene->CreateGameObject<CPenguinCloseAttack>("AttackEffect");
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

void CBossPenguin::AIAttackSpecific(float DeltaTime)
{
	if (m_Jump)
		return;

	if (m_IsAttacking)
		return;

	// Player가 왼쪽에 있다는 것
	if (m_Scene->GetPlayerObject()->GetWorldPos().x - GetWorldPos().x < 0.f)
		m_ObjectMoveDir.x = -1.f;
	else
		m_ObjectMoveDir.x = 1.f;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer < m_FarAttackDistance)
	{
		if (m_FarAttackLimitTime > 0.f)
			return;

		m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

		ChangeFarAttackAnimation();

		m_IsAttacking = true;

		// 이동안은, 상대 Player 에게 돌진하게 한다.
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

		ChangeCloseAttackAnimation();
	}
}

void CBossPenguin::AITraceSpecific(float DeltaTime)
{
	if (m_IsAttacking)
		return;

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
			m_JumpVelocity = 115.f;
			m_JumpLimitTime = m_JumpLimitTimeMax;
			m_MonsterMoveVelocity = 200.f;
			m_JumpAccel = 1.5f;
			ChangeJumpAttackAnimation();
			m_InitTraceDir = m_TraceDir;
		}
	}
	else if (DistToPlayer < m_DashRunDistance)
	{
		// 그냥 계속 그대로 진행 (ChangeTraceAnimation)
	}
}

void CBossPenguin::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CBossPenguin::ChangeCloseAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
}

void CBossPenguin::ChangeJumpAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftJump");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightJump");
}

void CBossPenguin::UpdateJumpAction(float DeltaTime)
{
	// 설령 점프 중이 아니라고 하더라도,
	// Trace Dir 만큼 계속 이동 시킨다.
	if (m_Jump)
	{
		AddWorldPos(Vector3(m_InitTraceDir.x, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);
	}

	if (m_JumpLimitTime >= 0.f)
	{
		m_JumpLimitTime -= DeltaTime;
	}
}

void CBossPenguin::UpdateFarAttackAction(float DeltaTime)
{
	if (m_FarAttackLimitTime > 0.f)
	{
		m_IsAttacking = true;

		m_FarAttackLimitTime -= DeltaTime;

		AddWorldPos(Vector3(m_FarAttackTraceDir.x, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (m_FarAttackLimitTime < 0.01f)
		{
			m_FarAttackLimitTime = -1.f;

			m_IsAttacking = false;

			m_MonsterMoveVelocity = m_InitMoveVelocity;
		}
	}
}

void CBossPenguin::SetObjectLand()
{
	CBossMonster::SetObjectLand();

	m_MonsterMoveVelocity = m_InitMoveVelocity;

	m_Jump = false;

	m_JumpAccel = m_InitJumpAccel;
}

void CBossPenguin::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

