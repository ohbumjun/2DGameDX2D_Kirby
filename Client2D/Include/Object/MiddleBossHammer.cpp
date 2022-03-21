#include "MiddleBossHammer.h"
#include "HammerGorillaFarAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"
#include "HammerGorillaCloseAttack.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer() :
	m_JumpEnable(false),
	m_JumpLimitTimeMax(8.f),
	m_JumpDistance(1000.f),
	m_DashRunDistance(800.f)
{
	SetTypeID<CMiddleBossHammer>();
	m_DashDistance = 1000.f;
	m_JumpVelocity = 50.f;
	m_AttackDistance = 600.f;
	m_IsGroundObject = true;
	m_CloseAttackDistance = 250.f;
	m_FarAttackDistance = 600.f;
	m_HPMax = 1000.f;
	m_HPMax = 1000.f;
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
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CMiddleBossHammer::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CMiddleBossHammer::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CMiddleBossHammer::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CMiddleBossHammer::FarAttack);

	// Hit ���Ŀ���, �ٷ� Idle �� �Ѿ�� �����Ѵ�.
	/*
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
		*/

	m_InitMoveVelocity = m_MonsterMoveVelocity;

	m_InitJumpAccel = m_JumpAccel;

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

	UpdateJumpAction(DeltaTime);
}

void CMiddleBossHammer::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CMiddleBossHammer* CMiddleBossHammer::Clone()
{
	return new CMiddleBossHammer(*this);
}

void CMiddleBossHammer::FarAttack()
{
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

		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->SetRightAttackDir(0.f);
	}

	AttackEffect->SetMonsterOwner(this);

	AttackEffect->SetJumpVelocity(80.f);

	AttackEffect->SetPhysicsSimulate(true);

	AttackEffect->JumpStart();

	AttackEffect->SetCreateMultipleEffectAfterWards(true);

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	m_IsAttacking = false;

	ChangeIdleAnimation();
}

void CMiddleBossHammer::CloseAttack()
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

	AttackEffect->AddRelativeRotationZ(90.f);

	AttackEffect->SetLifeTime(0.5f);

	m_IsAttacking = false;

	ChangeIdleAnimation();
}

void CMiddleBossHammer::AIAttackSpecific(float DeltaTime)
{
	if (m_Jump)
		return;

	if (m_IsAttacking)
		return;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer < m_FarAttackDistance)
	{
		if (m_FarAttackLimitTime > 0.f)
			return;

		m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

		ChangeFarAttackAnimation();
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

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
		// �׳� ��� �״�� ���� (ChangeTraceAnimation)
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

void CMiddleBossHammer::UpdateJumpAction(float DeltaTime)
{
	// ���� ���� ���� �ƴ϶�� �ϴ���,
	// Trace Dir ��ŭ ��� �̵� ��Ų��.
	if (m_Jump)
	{
		AddWorldPos(Vector3(m_InitTraceDir.x, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);
	}

	if (m_JumpLimitTime >= 0.f)
	{
		m_JumpLimitTime -= DeltaTime;
	}
}

void CMiddleBossHammer::SetObjectLand()
{
	CBossMonster::SetObjectLand();

	m_MonsterMoveVelocity = m_InitMoveVelocity;

	m_Jump = false;

	m_JumpAccel = m_InitJumpAccel;
}

void CMiddleBossHammer::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

