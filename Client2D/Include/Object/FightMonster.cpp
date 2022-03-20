#include "FightMonster.h"
#include "FightMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CFightMonster::CFightMonster() 
{
	SetTypeID<CFightMonster>();

	m_AbilityState = Ability_State::Fight;

	m_DashDistance = 600.f;

	m_AttackDistance = 350.f;
}

CFightMonster::CFightMonster(const CFightMonster& Monster) : CAbilityMonster(Monster)
{}

CFightMonster::~CFightMonster()
{}

void CFightMonster::Start()
{
	CAbilityMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

	// Ready�� ���, �ſ� �� PlayTime �� �����ϰ�
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(1.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetInitPauseTime(1.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CFightMonster::Attack);

	// EndCallback�� �����Ѵ�.
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(1.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetInitPauseTime(1.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CFightMonster::Attack);

	m_IsAttacking = false;
}

bool CFightMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Fight", TEXT("Ability_Fight.anim"));

	SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CFightMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CFightMonster::PostUpdate(float DeltaTime)
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

CFightMonster* CFightMonster::Clone()
{
	return new CFightMonster(*this);
}

void CFightMonster::Attack()
{
	CAbilityMonster::Attack();

	// ������ ���� �ִٸ� 
	if (m_ObjectMoveDir.x < 0.f)
	{
		// ���
		CFightMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CFightMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir();
	}
	// ���������� ���� �ִٸ� 
	else
	{
		CFightMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CFightMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir();
	}

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	ChangeIdleAnimation();
}
