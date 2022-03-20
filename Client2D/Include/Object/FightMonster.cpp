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

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(1.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetInitPauseTime(1.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CFightMonster::Attack);

	// EndCallback도 세팅한다.
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

	// 왼쪽을 보고 있다면 
	if (m_ObjectMoveDir.x < 0.f)
	{
		// 가운데
		CFightMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CFightMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir();
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CFightMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CFightMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir();
	}

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	ChangeIdleAnimation();
}
