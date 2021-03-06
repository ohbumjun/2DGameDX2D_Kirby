#include "SwordKirbyState.h"
#include "Component/ColliderCircle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"
#include "../Object/FireAttackBackEffect.h"
#include "Component/ColliderBox2D.h"
#include "../Object/SwordBoomerang.h"

class CFightMonsterAttack;

CSwordKirbyState::CSwordKirbyState() :
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_GoUpState(false)
{
	m_ExtraAttackAbility = 5.f;
}

CSwordKirbyState::~CSwordKirbyState()
{}

void CSwordKirbyState::Attack()
{
	SetWorldScale(Vector3(m_InitWorldScale.x * 1.8f, m_InitWorldScale.y * 1.8f, m_InitWorldScale.z));

	m_Player->ChangePlayerAttackAnimation();

	m_Player->WeakJump();
}

void CSwordKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	SetWorldScale(Vector3(m_InitWorldScale.x * 1.8f, m_InitWorldScale.y * 1.3f, m_InitWorldScale.z));

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	// 왼쪽을 보고 있다면
	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::SwordFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::SwordFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerFallDownAttackAnimation();
}

void CSwordKirbyState::GoUpAttack()
{
	if (m_FallAttackState)
		return;

	if (m_GoUpState)
		return;
	
	m_GoUpState = true;

	SetWorldScale(Vector3(m_InitWorldScale.x * 3.5f, m_InitWorldScale.y * 2.5f, m_InitWorldScale.z));

	m_InitColliderLength = m_Player->GetBodyCollider()->GetInfo().Length;

	m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x * 3.5f, m_InitColliderLength.y * 2.0f);

	m_Player->GetBodyCollider()->SetCollisionProfile("PlayerAttack");

	m_GoUpReachedTop = false;

	m_GoUpDist = 0.f;

	m_Player->SetPhysicsSimulate(false);
}

void CSwordKirbyState::SpecialAttack()
{
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	// 왼쪽 공격
	if (PlayerMoveDir.x < 0.f)
	{
		CSwordBoomerang* AttackEffect = m_Scene->CreateGameObject<CSwordBoomerang>("Attack1");
		AttackEffect->SetWorldPos(GetWorldPos().x - 50.f, GetWorldPos().y + 60.f, GetWorldPos().z);
		AttackEffect->SetAttackDir(-1.f);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);
	}
	else
	{
		CSwordBoomerang* AttackEffect = m_Scene->CreateGameObject<CSwordBoomerang>("Attack1");
		AttackEffect->SetWorldPos(GetWorldPos().x + 50.f, GetWorldPos().y + 60.f, GetWorldPos().z);
		AttackEffect->SetAttackDir(1.f);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

	// 다시 원래의 Size 로 세팅하기 
	SetWorldScale(80.f, 100.f, 1.f);
}

void CSwordKirbyState::UltimateAttack()
{
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	float YPosStart = m_Player->GetWorldPos().y + 510.f;

	for (int i = 1; i <= 5; i++)
	{
		CSwordBoomerang* AttackEffect = m_Scene->CreateGameObject<CSwordBoomerang>("Attack1");

		float YPos = YPosStart - (i * 1) * 170.f;

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 1.5f);

		if (PlayerMoveDir.x < 0.f)
		{
			AttackEffect->SetAttackDir(-1.f); //

			float XPos = GetWorldPos().x - ((float)rand() / (float)RAND_MAX) * 500.f;

			AttackEffect->SetWorldPos(XPos, YPos, GetWorldPos().z);
		}
		else
		{
			AttackEffect->SetAttackDir(1.f);

			float XPos = GetWorldPos().x + ((float)rand() / (float)RAND_MAX) * 500.f;

			AttackEffect->SetWorldPos(XPos, YPos, GetWorldPos().z);
		}
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

	// 다시 원래의 Size 로 세팅하기 
	SetWorldScale(80.f, 100.f, 1.f);
}

void CSwordKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpState)
	{
		const float Dist = DeltaTime * 1000.f;

		m_Player->SetAttackEnable(true);

		if (m_Animation->GetCurrentAnimation()->GetName() != "RightUpAttack" &&
			m_Animation->GetCurrentAnimation()->GetName() != "LeftUpAttack")
		{
			if (m_Player->GetObjectMoveDir().x < 0.f)
			{
				m_Animation->ChangeAnimation("LeftUpAttack");
			}
			else
			{
				m_Animation->ChangeAnimation("RightUpAttack");
			}
		}

		if (!m_GoUpReachedTop)
		{
			AddWorldPos(Vector3(0.f, 1.f, 0.f) * Dist);

			m_GoUpDist += Dist;

			if (m_GoUpDist >= 500.f)
			{
				m_GoUpReachedTop = true;

				m_GoUpDist = 0.f;
			}
		}
		else
		{
			AddWorldPos(Vector3(0.f, -1.f, 0.f) * Dist);

			m_GoUpDist += Dist;

			if (m_Player->IsBottomCollided() || m_GoUpDist >= 900.f)
			{
				m_GoUpState = false;

				m_Player->SetAttackEnd();

				m_Player->ChangePlayerWalkAnimation();

				m_Player->GetBodyCollider()->SetCollisionProfile("Player");

				m_Player->SetPhysicsSimulate(true);

				SetWorldScale(m_InitWorldScale);

				m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x, m_InitColliderLength.y);

				// 한번 더 공격 
				NormalAttackCallback();
			}
		}

		m_CloneEffectToggleTime += DeltaTime;

		if (m_CloneEffectToggleTime >= m_CloneEffectToggleMaxTime)
		{
			m_Player->MakePlayerCloneEffect();
			m_CloneEffectToggleTime = 0.f;
		}
	}
}

void CSwordKirbyState::UpdateFallAttack(float DeltaTime)
{
	if (m_FallAttackTime > 0.f)
	{
		m_FallAttackTime -= DeltaTime;

		m_Player->ChangePlayerFallDownAttackAnimation();

		m_Player->SetAttackEnable(true);

		if (m_Player->GetObjectMoveDir().x > 0)
		{
			AddWorldPos(Vector3(1.f, 1.f * -1.f, 0.f) * DeltaTime * 650.f);
		}
		else
		{
			AddWorldPos(Vector3(1.f * -1.f, 1.f * -1.f, 0.f) * DeltaTime * 650.f);
		}

		m_CloneEffectToggleTime += DeltaTime;

		if (m_CloneEffectToggleTime >= m_CloneEffectToggleMaxTime)
		{
			m_Player->MakePlayerCloneEffect();
			m_CloneEffectToggleTime = 0.f;
		}

		if (m_FallAttackTime <= 0.f)
		{
			m_FallAttackState = false;

			m_Player->SetAttackEnable(false);

			m_Player->ChangePlayerFallAnimation();

			SetWorldScale(m_InitWorldScale);
		}
	}
}

void CSwordKirbyState::UpdateSpecialAttack(float DeltaTime)
{
	if (!m_IsSpecialAttack)
		return;

	m_SpecialAttackTime += DeltaTime;
}

void CSwordKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CSwordKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"Kirby_Sword", TEXT("Kirby_Cutter.anim"));

	SetAnimationInstance(AnimationInstance);

	m_Animation->Play();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CSwordKirbyState::NormalAttackCallback);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CSwordKirbyState::NormalAttackCallback);

	// m_InitWorldScale = GetWorldScale();
	m_InitWorldScale = Vector3(80.f, 100.f, 1.f);

	return true;
}

void CSwordKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);

	UpdateAttackGoUpState(DeltaTime);

	UpdateFallAttack(DeltaTime);
}

void CSwordKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CSwordKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CSwordKirbyState::Render()
{
	CKirbyState::Render();
}

void CSwordKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

void CSwordKirbyState::NormalAttackCallback()
{
	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	SetWorldScale(m_InitWorldScale);

	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("NormalFireAttack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Sword);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("NormalFireAttack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Sword);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerFallAnimation();
}
