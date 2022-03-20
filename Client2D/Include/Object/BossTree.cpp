#include "BossTree.h"
#include "HammerGorillaFarAttack.h"
#include <Component/SpriteComponent.h>
#include <Scene/Scene.h>
#include "Engine.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Apple.h"
#include "Component/ColliderCircle.h"

class CAnimationSequence2DInstance;

CBossTree::CBossTree()  :
	m_FarAttackLimitTime(0.f),
	m_FarAttackLimitTimeMax(5.f)
{

	SetTypeID<CBossTree>();
	m_DashDistance = 1000.f;
	m_JumpVelocity = 50.f;
	m_IsGroundObject = false;
	m_PhysicsSimulate = false;

	m_AttackDistance = 900.f;
	m_CloseAttackDistance = 350.f;
	m_FarAttackDistance = 900.f;
}

CBossTree::CBossTree(const CBossTree& Monster) : CBossMonster(Monster)
{}

CBossTree::~CBossTree()
{}

void CBossTree::Start()
{
	CBossMonster::Start();

	m_PhysicsSimulate = false;

	m_IsGround = false;

	m_IsGroundObject = false;

	m_Sprite->GetAnimationInstance()->Play();

	// Close Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CBossTree::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CBossTree::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CBossTree::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CBossTree::FarAttack);

	// Hit 이후에는, 바로 Idle 로 넘어가게 세팅한다.
	/*
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
		*/

	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_RootComponent->GetWorldScale().x * 0.7f);

	m_IsAttacking = false;
}

bool CBossTree::Init()
{
	if (!CBossMonster::Init())
		return false;

	return true;
}

void CBossTree::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	m_MonsterMoveVelocity = 0.f;

	m_ObjectMoveDir = Vector3(-1.f, 0.f, 0.f);

	if (m_FarAttackLimitTime > 0.f)
	{
		m_FarAttackLimitTime -= DeltaTime;
	}
}

void CBossTree::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CBossTree* CBossTree::Clone()
{
	return new CBossTree(*this);
}

void CBossTree::FarAttack()
{
	if (m_IsAttacking)
		return;

	if (m_FarAttackLimitTime > 0.f)
		return;

	m_IsAttacking = true;

	for (int i = 0; i < 3; i++)
	{
		float XPos = ((float)(rand()) / (float)(RAND_MAX)) * (float)CEngine::GetInst()->GetResolution().Width;
		float YPos = (float)CEngine::GetInst()->GetResolution().Height;

		CApple* AttackEffect = m_Scene->CreateGameObject<CApple>("Attack");

		AttackEffect->SetWorldPos(XPos, YPos, GetWorldPos().z);

		AttackEffect->SetPhysicsSimulate(true);
	}

	m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	ChangeIdleAnimation();
}

void CBossTree::CloseAttack()
{
	/*
	// Attack Back Effect
	CHammerGorillaCloseAttack* AttackEffect = m_Scene->CreateGameObject<CHammerGorillaCloseAttack>("AttackEffect");
	//
	if (m_ObjectMoveDir.x < 0.f)
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x - GetWorldScale().x * 0.3f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);
	}
	else
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x + GetWorldScale().x * 0.3f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);
	}

	AttackEffect->AddRelativeRotationZ(90.f);

	AttackEffect->SetLifeTime(0.5f);

	// AttackEffect->SetAttackDirX(0.f);
	*/
}

void CBossTree::AIAttackSpecific(float DeltaTime)
{
	if (m_Jump)
		return;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer < m_FarAttackDistance)
	{
		ChangeFarAttackAnimation();
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		ChangeCloseAttackAnimation();
	}
}

void CBossTree::AITraceSpecific(float DeltaTime)
{
	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());
}

void CBossTree::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CBossTree::ChangeCloseAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
}

void CBossTree::ChangeJumpAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftJump");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightJump");
}

void CBossTree::ChangeJumpEndAttackAnimation()
{}

void CBossTree::ChangeToIdleAfterHit()
{}

void CBossTree::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

