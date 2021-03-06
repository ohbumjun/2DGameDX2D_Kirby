#include "BombMonster.h"
#include "BombMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CBombMonster::CBombMonster()
{
	SetTypeID<CBombMonster>();

}

CBombMonster::CBombMonster(const CBombMonster& Monster) : CAbilityMonster(Monster)
{}

CBombMonster::~CBombMonster()
{}

void CBombMonster::Start()
{
	CAbilityMonster::Start();

	Monster_Stat* Stat = SetExcelStat(L"Bomb");

	m_AbilityState = Stat->m_AbilityState;

	// m_PhysicsSimulate = true;
	// m_IsGroundObject = true;
	// m_AbilityState = Ability_State::Bomb;
	// m_DashDistance = 550.f;
	// m_AttackDistance = 450.f;

	m_IsGround = true;

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CBombMonster::Attack);

	// EndCallback도 세팅한다.
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CBombMonster::Attack);

	m_IsAttacking = false;
}

bool CBombMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Bomb", TEXT("Ability_Bomb.anim"));

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

	// 왼쪽을 보고 있다면 
	if (m_ObjectMoveDir.x < 0.f)
	{
		CBombMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CBombMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir();
		AttackEffect->ApplyJumpEffect();
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CBombMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CBombMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir();
		AttackEffect->ApplyJumpEffect();
	}

	m_Scene->GetResource()->SoundPlay("AbilityAttack");
}
