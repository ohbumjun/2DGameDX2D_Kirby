#include "BombMonster.h"
#include "BombMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CBombMonster::CBombMonster()
{
	SetTypeID<CBombMonster>();

	m_AbilityState = Ability_State::Beam;

	m_DashDistance = 550.f;

	m_AttackDistance = 450.f;
}

CBombMonster::CBombMonster(const CBombMonster& Monster) : CAbilityMonster(Monster)
{}

CBombMonster::~CBombMonster()
{}

void CBombMonster::Start()
{
	CAbilityMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

	// Ready�� ���, �ſ� �� PlayTime �� �����ϰ�
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CBombMonster::Attack);

	// EndCallback�� �����Ѵ�.
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CBombMonster::Attack);

	m_IsAttacking = false;
}

bool CBombMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CBombMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CBombMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CBombMonster* CBombMonster::Clone()
{
	return new CBombMonster(*this);
}

void CBombMonster::Attack()
{
	if (m_IsAttacking)
		return;

	if (m_AttackLimitTime > 0.f)
		return;

	m_IsAttacking = true;

	m_AttackLimitTime = m_AttackLimitTimeMax;

	// ������ ���� �ִٸ� 
	if (m_ObjectMoveDir.x < 0.f)
	{
		CBombMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CBombMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir();
		AttackEffect->ApplyJumpEffect();
	}
	// ���������� ���� �ִٸ� 
	else
	{
		CBombMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CBombMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir();
		AttackEffect->ApplyJumpEffect();
	}
}
