#include "BeamMonster.h"
#include "BeamMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>

#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CBeamMonster::CBeamMonster() 
{
	SetTypeID<CBeamMonster>();

	m_AbilityState = Ability_State::Beam;

	m_DashDistance = 550.f;

	m_AttackDistance = 450.f;
}

CBeamMonster::CBeamMonster(const CBeamMonster& Monster) : CAbilityMonster(Monster)
{}

CBeamMonster::~CBeamMonster()
{}

void CBeamMonster::Start()
{
	CAbilityMonster::Start();

	Monster_Stat* Stat = SetExcelStat(L"Beam");

	m_AbilityState = Stat->m_AbilityState;

	// m_PhysicsSimulate = true;
	// m_IsGroundObject = true;

	m_IsGround = true;


	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CBeamMonster::Attack);

	// EndCallback도 세팅한다.
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CBeamMonster::Attack);

	m_IsAttacking = false;
}

bool CBeamMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	LoadAnimationInstance("Beam", TEXT("Ability_Beam.anim"));

	SetCurrentAnimation("RightIdle");

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CBeamMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CBeamMonster::PostUpdate(float DeltaTime)
{
	CAbilityMonster::PostUpdate(DeltaTime);
}

CBeamMonster* CBeamMonster::Clone()
{
	return new CBeamMonster(*this);
}

void CBeamMonster::Attack()
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
		CBeamMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CBeamMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f ,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir();
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CBeamMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CBeamMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir();
	}
}
