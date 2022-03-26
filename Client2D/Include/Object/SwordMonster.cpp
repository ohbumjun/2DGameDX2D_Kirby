#include "SwordMonster.h"
#include "SwordMonsterAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"

class CAnimationSequence2DInstance;

CSwordMonster::CSwordMonster()
{
	SetTypeID<CSwordMonster>();

	m_AbilityState = Ability_State::Fire;

	m_DashDistance = 800.f;

	m_AttackDistance = 450.f;

	m_JumpVelocity = 50.f;
}
CSwordMonster::~CSwordMonster()
{}

void CSwordMonster::Start()
{
	CAbilityMonster::Start();

	m_PhysicsSimulate = true;

	m_IsGround = true;

	m_IsGroundObject = true;

	// Ready의 경우, 매우 긴 PlayTime 을 세팅하고
	m_Sprite->GetAnimationInstance()->Play();

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(this, &CSwordMonster::Attack);

	// EndCallback도 세팅한다.
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(this, &CSwordMonster::Attack);

	m_IsAttacking = false;
}

bool CSwordMonster::Init()
{
	if (!CAbilityMonster::Init())
		return false;

	// m_Sprite->CreateAnimationInstance<CMonsterAnimation>();

	return true;
}

void CSwordMonster::Update(float DeltaTime)
{
	CAbilityMonster::Update(DeltaTime);
}

void CSwordMonster::PostUpdate(float DeltaTime)
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


void CSwordMonster::Attack()
{
	CAbilityMonster::Attack();

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
		CSwordMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CSwordMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);

		// 가운데
		AttackEffect = m_Scene->CreateGameObject<CSwordMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir(0.f);

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CSwordMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLeftAttackDir(TraceDownDir.y);

	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CSwordMonsterAttack* AttackEffect = m_Scene->CreateGameObject<CSwordMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);

		// 가운데
		AttackEffect = m_Scene->CreateGameObject<CSwordMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir(0.f);

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CSwordMonsterAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
	}

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	ChangeIdleAnimation();
}

void CSwordMonster::AITraceSpecific(float DeltaTime)
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
