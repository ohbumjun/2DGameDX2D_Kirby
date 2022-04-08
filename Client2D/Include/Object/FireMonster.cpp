#include "FireMonster.h"
#include "FireMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/CameraManager.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CFireMonster::CFireMonster() 
{
	SetTypeID<CFireMonster>();

	
}

CFireMonster::CFireMonster(const CFireMonster& Monster) : CAbilityMonster(Monster)
{}

CFireMonster::~CFireMonster()
{}

void CFireMonster::Start()
{
	CAbilityMonster::Start();

	// m_PhysicsSimulate = true;
	// m_IsGroundObject = true;
	// m_AbilityState = Ability_State::Fire;
	// m_DashDistance = 800.f;
	// m_AttackDistance = 450.f;
	// m_JumpVelocity = 50.f;

	Monster_Stat* Stat = SetExcelStat(L"Fire");

	m_AbilityState = Stat->m_AbilityState;


	m_IsGround = true;

	// Ready�� ���, �ſ� �� PlayTime �� �����ϰ�
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CFireMonster::Attack);

	// EndCallback�� �����Ѵ�.
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CFireMonster::Attack);

	m_IsAttacking = false;
}

bool CFireMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CFireMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CFireMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);

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

CFireMonster* CFireMonster::Clone()
{
	return new CFireMonster(*this);
}

void CFireMonster::Attack()
{
	CAbilityMonster::Attack();

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
		CFireMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);

		// ���
		AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir(0.f);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir(TraceDownDir.y);

	}
	// ���������� ���� �ִٸ� 
	else
	{
		CFireMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);

		// ���
		AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir(0.f);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
	}

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	ChangeIdleAnimation();

	m_Scene->GetResource()->SoundPlay("AbilityAttack");
}

void CFireMonster::AITraceSpecific(float DeltaTime)
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
