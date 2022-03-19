#include "MiddleBossHammer.h"
#include "HammerGorillaFarAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CMiddleBossHammer::CMiddleBossHammer() :
	m_IsAttacking(false),
	m_AttackResetTime(0.f),
	m_AttackResetTimeMax(2.f)
{
	SetTypeID<CMiddleBossHammer>();

	m_DashDistance = 800.f;

	m_AttackDistance = 450.f;

	m_JumpVelocity = 50.f;
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

	// Ready�� ���, �ſ� �� PlayTime �� �����ϰ�
	m_Sprite->GetAnimationInstance()->Play();

	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CMiddleBossHammer::Attack);

	// EndCallback�� �����Ѵ�.
	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.f);
	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CMiddleBossHammer::Attack);

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
}

CMiddleBossHammer* CMiddleBossHammer::Clone()
{
	return new CMiddleBossHammer(*this);
}

void CMiddleBossHammer::Attack()
{
	if (m_IsAttacking)
		return;

	m_IsAttacking = true;

	Vector3 PlayerPos = m_Scene->GetPlayerObject()->GetWorldPos();

	// ��
	Vector3 TraceUpDir = Vector3(PlayerPos.x, PlayerPos.y + 100.f, PlayerPos.z) - GetWorldPos();
	TraceUpDir.Normalize();

	// �Ʒ�
	Vector3 TraceDownDir = Vector3(PlayerPos.x, PlayerPos.y - 100.f, PlayerPos.z) - GetWorldPos();
	TraceDownDir.Normalize();


	// ������ ���� �ִٸ� 
	if (m_ObjectMoveDir.x < 0.f)
	{
		// �� 
		CHammerGorillaFarAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);

		// ���
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetLeftAttackDir(0.f);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetLeftAttackDir(TraceDownDir.y);

	}
	// ���������� ���� �ִٸ� 
	else
	{
		CHammerGorillaFarAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);

		// ���
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetRightAttackDir(0.f);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
	}

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	ChangeIdleAnimation();
}

