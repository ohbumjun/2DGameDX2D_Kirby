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

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CMiddleBossHammer::Attack);

	// EndCallback도 세팅한다.
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

	// 위
	Vector3 TraceUpDir = Vector3(PlayerPos.x, PlayerPos.y + 100.f, PlayerPos.z) - GetWorldPos();
	TraceUpDir.Normalize();

	// 아래
	Vector3 TraceDownDir = Vector3(PlayerPos.x, PlayerPos.y - 100.f, PlayerPos.z) - GetWorldPos();
	TraceDownDir.Normalize();


	// 왼쪽을 보고 있다면 
	if (m_ObjectMoveDir.x < 0.f)
	{
		// 위 
		CHammerGorillaFarAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);

		// 가운데
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetLeftAttackDir(0.f);

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetLeftAttackDir(TraceDownDir.y);

	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CHammerGorillaFarAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);

		// 가운데
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetRightAttackDir(0.f);

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CHammerGorillaFarAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetFireOwner(this);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
	}

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	ChangeIdleAnimation();
}

