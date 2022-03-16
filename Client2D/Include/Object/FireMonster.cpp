#include "FireMonster.h"
#include "FireMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CFireMonster::CFireMonster() :
	m_IsAttacking(false)
{
	SetTypeID<CFireMonster>();

	m_AbilityState = Ability_State::Beam;
}

CFireMonster::CFireMonster(const CFireMonster& Monster) : CAbilityMonster(Monster)
{}

CFireMonster::~CFireMonster()
{}

void CFireMonster::Start()
{
	CAbilityMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

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
}

CFireMonster* CFireMonster::Clone()
{
	return new CFireMonster(*this);
}

void CFireMonster::Attack()
{
	if (m_IsAttacking)
		return;

	m_IsAttacking = true;

	// ������ ���� �ִٸ� 
	if (m_ObjectMoveDir.x < 0.f)
	{
		m_AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		m_AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		m_AttackEffect->SetFireOwner(this);
		m_AttackEffect->SetLeftAttackDir();
	}
	// ���������� ���� �ִٸ� 
	else
	{
		m_AttackEffect = m_Scene->CreateGameObject<CFireMonsterAttack>("Attack");
		m_AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		m_AttackEffect->SetFireOwner(this);
		m_AttackEffect->SetRightAttackDir();
	}
}
