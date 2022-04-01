#include "FightKirbyState.h"
#include <Component/ColliderBox2D.h>
#include "Component/ColliderCircle.h"
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
	m_SpecialAttackTimeMax(2.0f),
	m_FallAttackState(false),
	m_IsSpecialAttack(false),
	m_GoUpState(false)
{
	m_ExtraAttackAbility = 15.f;
}

CFightKirbyState::CFightKirbyState(const CFightKirbyState& Kirby) : CKirbyState(Kirby)
{}

CFightKirbyState::~CFightKirbyState()
{}

void CFightKirbyState::Attack()
{
	m_Player->ChangePlayerAttackAnimation();
}

void CFightKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	// 왼쪽을 보고 있다면
	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x ,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 위
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y + 50.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 위 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x,
			GetWorldPos().y + 100.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y - 50.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 아래 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x,
			GetWorldPos().y - 100.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 위
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y + 50.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 위 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x,
			GetWorldPos().y + 100.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y - 50.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// 아래 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightFall);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x,
			GetWorldPos().y - 100.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerFallAnimation();
}

void CFightKirbyState::GoUpAttack()
{
	if (m_FallAttackState)
		return;

	m_GoUpState = true;

	m_GoUpTime = m_GoUpTimeMax;

	m_InitAttackAbility = m_ExtraAttackAbility;

	m_ExtraAttackAbility = 30.f;

	m_InitWorldScale = GetWorldScale();

	m_InitColliderLength = m_Player->GetBodyCollider()->GetInfo().Length;

	m_Player->GetBodyCollider()->SetCollisionProfile("PlayerAttack");

	// 위 방향으로 크게 쏜다
	CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
	AttackEffect->SetAttackType(KirbyAttackEffect_Type::Fight);
	AttackEffect->SetLeftAttackDir(1.f);
	AttackEffect->SetAttackDirX(0.f);
	AttackEffect->AddRelativeRotationZ(-90.f);
	AttackEffect->SetWorldScale(400.f, 400.f, 1.f);
	AttackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), AttackEffect->GetWorldScale().x * 0.5f);
	AttackEffect->SetKirbyOwner(this);
	AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	AttackEffect->SetDestroyWhenCollide(false);

	AttackEffect->SetWorldPos(GetWorldPos().x,
		GetWorldPos().y - GetWorldScale().y, GetWorldPos().z);
}

void CFightKirbyState::SpecialAttack()
{
	m_InitWorldScale =  GetWorldScale();

	m_InitColliderLength = m_Player->GetBodyCollider()->GetInfo().Length;

	if (m_Player->GetObjectMoveDir().x < 0.f)
	{
		m_Player->ChangeAnimation("LeftSpecialEffect");

		m_SpecialAttackDir = -1.f;

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - 150.f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("LeftSpecialEffect");

		// 위
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x -150.f,
			GetWorldPos().y + 200.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("LeftSpecialEffect");

		// 위 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + 150.f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("LeftSpecialEffect");

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + 150.f,
			GetWorldPos().y + 200.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("LeftSpecialEffect");

		// 아래 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x ,GetWorldPos().y + 100.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("LeftSpecialEffect");
	}
	else
	{
		m_Player->ChangeAnimation("RightSpecialEffect");

		m_SpecialAttackDir = 1.f;

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + 150.f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("RightSpecialEffect");

		// 위
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + 150.f,
			GetWorldPos().y + 200.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("RightSpecialEffect");

		// 위 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - 150.f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("RightSpecialEffect");

		// 아래
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x - 150.f,
			GetWorldPos().y + 200.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("RightSpecialEffect");

		// 아래 * 2
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FightSpecial);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 100.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
		AttackEffect->GetMainSprite()->GetAnimationInstance()->ChangeAnimation("RightSpecialEffect");
	}

	m_IsSpecialAttack = true;

	m_SpecialAttackTime = 0.f;

	// Player 의 Body Collider 를 Player Attack Profile 로 세팅한다.
	m_Player->GetBodyCollider()->SetCollisionProfile("PlayerAttack");
}

void CFightKirbyState::UltimateAttack()
{}

void CFightKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpTime > 0.f)
	{
		m_GoUpTime -= DeltaTime;

		m_Player->SetAttackEnable(true);

		SetWorldScale(Vector3(m_InitWorldScale.x * 3.f, m_InitWorldScale.y * 3.f, m_InitWorldScale.z));

		if (m_Player->GetObjectMoveDir().x > 0)
		{
			AddWorldPos(Vector3(0.5f, 1.f, 0.f) * DeltaTime * 500.f);
		}
		else
		{
			AddWorldPos(Vector3(0.5f * -1.f, 1.f, 0.f) * DeltaTime * 500.f);
		}

		m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x * 3.f, m_InitColliderLength.y * 2.f);

		if (m_GoUpTime <= 0.f)
		{
			m_GoUpState = false;

			SetWorldScale(m_InitWorldScale);

			m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x, m_InitColliderLength.y);

			m_Player->SetAttackEnable(false);

			m_ExtraAttackAbility = m_InitAttackAbility;

			m_Player->GetBodyCollider()->SetCollisionProfile("Player");
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

void CFightKirbyState::UpdateSpecialAttack(float DeltaTime)
{
	if (!m_IsSpecialAttack)
		return;

	m_SpecialAttackTime += DeltaTime;

	if (m_SpecialAttackDir < 0.f)
	{
		AddWorldPos(Vector3(-1.f, 0.f, 0.f) * DeltaTime * 700.f);
	}
	else
	{
		AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * 700.f);
	}

	SetWorldScale(Vector3(m_InitWorldScale.x * 1.2f, m_InitWorldScale.y * 1.2f, m_InitWorldScale.z));

	if (m_SpecialAttackTime >= m_SpecialAttackTimeMax)
	{
		m_IsSpecialAttack = false;

		m_SpecialAttackTime = 0.f;

		m_Player->SetAttackEnd();

		SetWorldScale(m_InitWorldScale);

		m_Player->GetBodyCollider()->SetCollisionProfile("Player");

		m_Player->ChangePlayerIdleAnimation();

		m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x, m_InitColliderLength.y);
	}
}

void CFightKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->Play();

	m_InitWorldScale = GetWorldScale();

	m_InitColliderLength = m_Player->GetBodyCollider()->GetInfo().Length;

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	m_Animation->FindAnimationSequence2DData("LeftSpecialEffect")->SetPlayTime(2.5f);
	m_Animation->FindAnimationSequence2DData("RightSpecialEffect")->SetPlayTime(2.5f);
}

bool CFightKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"Kirby_Fight", TEXT("Kirby_Fight.anim"));

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

	UpdateSpecialAttack(DeltaTime);
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
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
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
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}

	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

}
