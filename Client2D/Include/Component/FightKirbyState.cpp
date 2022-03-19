#include "FightKirbyState.h"

#include <Component/ColliderBox2D.h>

#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"

CFightKirbyState::CFightKirbyState() :
	m_GoUpTimeMax(0.4f),
	m_GoUpTime(0.f),
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_GoUpState(false)
{
}

CFightKirbyState::CFightKirbyState(const CFightKirbyState& Kirby) : CKirbyState(Kirby)
{}

CFightKirbyState::~CFightKirbyState()
{}

void CFightKirbyState::Attack()
{}

void CFightKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		// 위
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y + 20.f, GetWorldPos().z);

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y - 20.f, GetWorldPos().z);
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y + 20.f, GetWorldPos().z);

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y - 20.f, GetWorldPos().z);
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerFallAnimation();
}

void CFightKirbyState::GoUpAttack()
{
	if (m_FallAttackState)
		return;

	if (!m_GoUpState)
	{
		m_InitWorldScale = GetWorldScale();
	}

	m_GoUpState = true;

	m_GoUpTime = m_GoUpTimeMax;

	m_InitColliderLength = m_Player->GetBodyCollider()->GetInfo().Length;
}

void CFightKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpTime > 0.f)
	{
		m_GoUpTime -= DeltaTime;

		m_Player->SetAttackEnable(true);

		SetWorldScale(Vector3(m_InitWorldScale.x * 2.f, m_InitWorldScale.y * 2.f, m_InitWorldScale.z));

		if (m_Player->GetObjectMoveDir().x > 0)
		{
			AddWorldPos(Vector3(0.3f, 1.f, 0.f) * DeltaTime * 500.f);
		}
		else
		{
			AddWorldPos(Vector3(0.3f * -1.f, 1.f, 0.f) * DeltaTime * 500.f);
		}

		m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x * 2.f, m_InitColliderLength.y * 2.f);

		if (m_GoUpTime <= 0.f)
		{
			m_GoUpState = false;

			SetWorldScale(m_InitWorldScale);

			m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x, m_InitColliderLength.y);
		}
	}
}

void CFightKirbyState::UpdateFallAttack(float DeltaTime)
{
	if (m_FallAttackTime > 0.f)
	{
		m_FallAttackTime -= DeltaTime;

		m_Player->ChangePlayerFallDownAttackAnimation();

		m_Player->SetAttackEnable(true);

		if (m_Player->GetObjectMoveDir().x > 0)
		{
			AddWorldPos(Vector3(1.f, 1.f * -1.f, 0.f) * DeltaTime * 500.f);
		}
		else
		{
			AddWorldPos(Vector3(1.f * -1.f, 1.f * -1.f, 0.f) * DeltaTime * 500.f);
		}

		if (m_FallAttackTime <= 0.f)
		{
			m_FallAttackState = false;

			m_Player->SetAttackEnable(false);
		}
	}
}

void CFightKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->Play();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CFightKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fight", TEXT("Kirby_Fight.anim"));

	SetAnimationInstance(AnimationInstance);

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CFightKirbyState::NormalAttackCallback);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CFightKirbyState::NormalAttackCallback);

	m_Animation->Play();

	return true;
}

void CFightKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);

	UpdateAttackGoUpState(DeltaTime);

	UpdateFallAttack(DeltaTime);
}

void CFightKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CFightKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CFightKirbyState::Render()
{
	CKirbyState::Render();
}

void CFightKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

CFightKirbyState* CFightKirbyState::Clone()
{
	return new CFightKirbyState(*this);
}

void CFightKirbyState::NormalAttackCallback()
{
	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Fight);
		AttackEffect->SetLeftAttackDir(0.f);
		// CFightKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CFightKirbyNormalAttack>("Attack");
		// AttackEffect->SetLeftAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Fight);
		AttackEffect->SetRightAttackDir(0.f);
		// CFightKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CFightKirbyNormalAttack>("Attack");
		// AttackEffect->SetRightAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
		GetWorldPos().y, GetWorldPos().z);
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

}
