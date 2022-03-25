#include "BombKirbyState.h"
#include <Component/ColliderBox2D.h>
#include "Component/ColliderCircle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"

CBombKirbyState::CBombKirbyState() :
	m_GoUpTimeMax(0.4f),
	m_GoUpTime(0.f),
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_GoUpState(false)
{
	m_ExtraAttackAbility = 15.f;
}


CBombKirbyState::~CBombKirbyState()
{}

void CBombKirbyState::Attack()
{}

void CBombKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	m_Player->SetPhysicsSimulate(false);

	m_Player->ChangePlayerFallDownAttackAnimation();
}

void CBombKirbyState::GoUpAttack()
{
	if (m_FallAttackState)
		return;

	if (!m_GoUpState)
	{
		m_InitWorldScale = GetWorldScale();
	}

	m_GoUpState = true;

	m_GoUpTime = m_GoUpTimeMax;

	m_Player->SetPhysicsSimulate(false);

}

void CBombKirbyState::MakeFallDownBomb()
{
	if (m_FallDownBombMade)
		return;

	m_FallDownBombMade = true;

	// 가운데
	CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

	AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombFall);

	AttackEffect->SetLeftAttackDir(0.f);

	AttackEffect->SetAttackDirX(0.f);
	// CFightKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CFightKirbyNormalAttack>("Attack");
	// AttackEffect->SetLeftAttackDir(0.f);

	AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
		GetWorldPos().y, GetWorldPos().z);

	AttackEffect->ApplyJumpEffect();

	AttackEffect->ApplyCameraMove();

	AttackEffect->SetKirbyOwner(this);
}

void CBombKirbyState::SetFallAttackEnd()
{
	m_FallAttackState = false;

	m_Player->SetPhysicsSimulate(true);

	m_FallDownBombMade = false;

	m_Player->SetAttackEnd();

	m_Player->ChangePlayerFallAnimation();
}

void CBombKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpTime > 0.f)
	{
		m_GoUpTime -= DeltaTime;

		m_Player->SetAttackEnable(true);

		SetWorldScale(Vector3(m_InitWorldScale.x * 3.f, m_InitWorldScale.y * 3.f, m_InitWorldScale.z));

		m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x * 3.f, m_InitColliderLength.y * 2.f);

		if (m_GoUpTime <= 0.f)
		{
			m_GoUpState = false;

			SetWorldScale(m_InitWorldScale);

			m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x, m_InitColliderLength.y);

			m_Player->SetAttackEnable(false);

			m_ExtraAttackAbility = m_InitAttackAbility;
		}
	}
}

void CBombKirbyState::UpdateFallAttack(float DeltaTime)
{
	if (m_FallAttackState)
	{
		// m_Player->ChangePlayerFallDownAttackAnimation();

		m_Player->SetAttackEnable(true);

		// m_Player->SetAttackEnable(false);
	}
}

void CBombKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->Play();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CBombKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Bomb", 
		TEXT("Kirby_Bomb.anim"));

	SetAnimationInstance(AnimationInstance);

	// Jump
	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	// Fall Attack
	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetEndFunction(
		this, &CBombKirbyState::MakeFallDownBomb);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetEndFunction(
		this, &CBombKirbyState::MakeFallDownBomb);
	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetPlayTime(1.f);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetPlayTime(1.f);

	// Fall
	m_Animation->FindAnimationSequence2DData("RightFall")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftFall")->SetLoop(false);

	// Attack
	m_Animation->FindAnimationSequence2DData("RightAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(0.3f);


	m_Animation->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CBombKirbyState::NormalAttackCallback);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CBombKirbyState::NormalAttackCallback);

	m_Animation->Play();

	return true;
}

void CBombKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);

	UpdateAttackGoUpState(DeltaTime);

	UpdateFallAttack(DeltaTime);
}

void CBombKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CBombKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CBombKirbyState::Render()
{
	CKirbyState::Render();
}

void CBombKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

void CBombKirbyState::NormalAttackCallback()
{
	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Bomb);

		AttackEffect->SetLeftAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->ApplyJumpEffect();

		AttackEffect->SetKirbyOwner(this);
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Bomb);

		AttackEffect->SetRightAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->ApplyJumpEffect();

		AttackEffect->SetKirbyOwner(this);
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

}
