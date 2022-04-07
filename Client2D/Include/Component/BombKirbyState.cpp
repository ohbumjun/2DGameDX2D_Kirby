#include "BombKirbyState.h"
#include <Component/ColliderBox2D.h>
#include "Component/ColliderCircle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"
#include "../Object/BombKirbyThrowBomb.h"
#include "../Object/BombUltimate.h"
#include "../Object/EffectRandomStar.h"

CBombKirbyState::CBombKirbyState() :
	m_GoUpTimeMax(0.4f),
	m_GoUpTime(0.f),
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_GoUpState(false),
	m_AttackReady(false)
{
	m_ExtraAttackAbility = 15.f;
}


CBombKirbyState::~CBombKirbyState()
{}

void CBombKirbyState::Attack()
{
	if (!m_AttackReady) //
	{
		if (m_Player->GetObjectMoveDir().x < 0.f)
		{
			m_Player->ChangeAnimation("LeftAttackReady");

			m_ThrowReadyBomb = m_Scene->CreateGameObject<CBombKirbyThrowBomb>("ThrowBombCenter");

			m_ThrowReadyBomb->SetLeft();

			// m_ThrowReadyBomb->SetWorldPos(GetWorldPos().x - 100.f, GetWorldPos().y, GetWorldPos().z);

			m_Player->GetRootComponent()->AddChild(m_ThrowReadyBomb->GetRootComponent());

			m_ThrowReadyBomb->GetRootComponent()->SetRelativePos(0.f, 0.f, 0.f);
		}
		else
		{
			m_Player->ChangeAnimation("RightAttackReady");

			m_ThrowReadyBomb = m_Scene->CreateGameObject<CBombKirbyThrowBomb>("ThrowBombCenter");

			m_ThrowReadyBomb->SetRight();

			// m_ThrowReadyBomb->SetWorldPos(GetWorldPos());

			m_Player->GetRootComponent()->AddChild(m_ThrowReadyBomb->GetRootComponent());

			m_ThrowReadyBomb->GetRootComponent()->SetRelativePos(0.f, 0.f, 0.f);
		}

		m_AttackReady = true;

		// �ٽ� ���� ��ư�� ���� �� �־�� �ϱ� �����̴�.
		// m_Player->SetAttackEnd();
		m_Player->SetAttackReady(true);
	}
	else
	{
		m_Player->ChangePlayerAttackAnimation();

		m_AttackReady = false;

		m_Player->SetAttackReady(false);
	}
}

void CBombKirbyState::FallDownAttack()
{
	// ������ ���� ��ź�� ���� ��� �ִ� ���¶�� 
	if (m_AttackReady)
	{
		m_Player->ChangePlayerAttackAnimation();

		m_AttackReady = false;
	}
	else
	{
		if (m_GoUpState)
			return;

		m_FallAttackState = true;

		m_FallAttackTime = m_FallAttackTimeMax;

		m_Player->SetPhysicsSimulate(false);

		m_Player->ChangePlayerFallDownAttackAnimation();

		for (int i = 0; i < 3; i++)
		{
			CEffectRandomStar* Star = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
			Star->SetWorldPos(GetWorldPos().x, GetWorldPos().y - GetWorldScale().y * GetPivot().y, GetWorldPos().z);
		}
	}
}

void CBombKirbyState::GoUpAttack()
{
	if (m_FallAttackState)
		return;

	m_GoUpState = true;

	m_Player->ChangePlayerAttackAnimation();
}

void CBombKirbyState::SpecialAttack()
{
	// ������ ������ ���� �Ʒ��� 3���� �������� �����Ѵ�.
	if (m_Player->GetObjectMoveDir().x < 0.f)
	{
		// ��
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombSpecial);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

		AttackEffect->SetLeftAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x + 100.f, GetWorldPos().y + 700.f, 0.f);

		AttackEffect->SetKirbyOwner(this);

		// ��� 
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombSpecial);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

		AttackEffect->SetLeftAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 500.f, 0.f);

		AttackEffect->SetKirbyOwner(this);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombSpecial);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

		AttackEffect->SetLeftAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x + 100.f, GetWorldPos().y + 300.f, 0.f);

		AttackEffect->SetKirbyOwner(this);
	}
	// ���� ������ ������ �Ʒ��� 3���� �������� �����Ѵ�.
	else
	{
		// ��
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombSpecial);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

		AttackEffect->SetRightAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - 100.f, GetWorldPos().y + 700.f, 0.f);

		AttackEffect->SetKirbyOwner(this);

		// ���
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombSpecial);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

		AttackEffect->SetRightAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x, GetWorldPos().y + 500.f, 0.f);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombSpecial);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

		AttackEffect->SetRightAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - 100.f, GetWorldPos().y + 300.f, 0.f);

		AttackEffect->SetKirbyOwner(this);
	}


	m_Player->SetAttackEnd();

	m_Player->ChangePlayerIdleAnimation();
}

void CBombKirbyState::UltimateAttack()
{
	CBombUltimate* AttackEffect = m_Scene->CreateGameObject<CBombUltimate>("Attack");

	if (m_Player->GetObjectMoveDir().x < 0.f)
	{
		AttackEffect->SetLeftAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - 300.f, GetWorldPos().y + 700.f, 0.f);
	}
	else
	{
		AttackEffect->SetRightAttackDir(-1.f);

		AttackEffect->SetWorldPos(GetWorldPos().x + 300.f, GetWorldPos().y + 700.f, 0.f);
	}

	AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 5);

	AttackEffect->SetAttackDirX(0.f);

	AttackEffect->SetKirbyOwner(this);

	AttackEffect->GetColliderBody()->Enable(false);

	AttackEffect->ApplyCameraMove();
}

void CBombKirbyState::MakeFallDownBomb()
{
	if (m_FallDownBombMade)
		return;
	
	m_FallDownBombMade = true;

	// ���
	CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

	AttackEffect->SetAttackType(KirbyAttackEffect_Type::BombFall);

	// Damage �� ���ݴ� ũ�� �����Ѵ�.
	AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 2);

	AttackEffect->SetLeftAttackDir(-1.f);

	AttackEffect->SetAttackDirX(0.f);
	// CFightKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CFightKirbyNormalAttack>("Attack");

	AttackEffect->SetWorldPos(GetWorldPos());

	// AttackEffect->ApplyCameraMove();

	AttackEffect->SetKirbyOwner(this);

	for (int i = 0; i < 3; i++)
	{
		CEffectRandomStar* Star = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
		Star->SetWorldPos(GetWorldPos().x, GetWorldPos().y - GetWorldScale().y * GetPivot().y, GetWorldPos().z);
	}
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
	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetPlayTime(0.5f);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetPlayTime(0.5f);

	// Fall
	m_Animation->FindAnimationSequence2DData("RightFall")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftFall")->SetLoop(false);

	// Attack
	m_Animation->FindAnimationSequence2DData("RightAttack")->SetPlayTime(0.2f);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(0.2f);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CBombKirbyState::NormalAttackCallback);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CBombKirbyState::NormalAttackCallback);

	// AttackReady
	m_Animation->FindAnimationSequence2DData("RightAttackReady")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftAttackReady")->SetPlayTime(0.3f);

	m_Animation->Play();

	return true;
}

void CBombKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);

	UpdateAttackGoUpState(DeltaTime);

	UpdateFallAttack(DeltaTime);

	// �̹� Bomb �� ������ ���¿��� �ϴµ� ������ Bomb�� ���� ���� ���� �ִ�
	if (!m_AttackReady && m_ThrowReadyBomb)
	{
		m_Player->GetRootComponent()->DeleteChild(m_ThrowReadyBomb->GetRootComponent());
	}
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
	if (m_ThrowReadyBomb)
	{
		m_Player->GetRootComponent()->DeleteChild(m_ThrowReadyBomb->GetRootComponent());
		m_ThrowReadyBomb->Destroy();
		m_ThrowReadyBomb = nullptr;
	}

	// ���߿��� ���� Bomb �� ���
	if (m_GoUpState)
	{
		m_GoUpState = false;
	}

	// ������ ���� �ִٸ�
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// ���
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Bomb);

		AttackEffect->SetLeftAttackDir(0.f);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		AttackEffect->SetWorldPos(GetWorldPos().x ,GetWorldPos().y + GetWorldScale().y * GetPivot().y, GetWorldPos().z);

		AttackEffect->ApplyJumpEffect();

		AttackEffect->SetJumpVelocity(40.f + ((float)rand() / (float)(RAND_MAX)) * 30.f);

		AttackEffect->SetKirbyOwner(this);
	}
	// ���������� ���� �ִٸ� 
	else
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Bomb);

		AttackEffect->SetRightAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x,GetWorldPos().y + GetWorldScale().y * GetPivot().y, GetWorldPos().z);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		AttackEffect->ApplyJumpEffect();

		AttackEffect->SetJumpVelocity(40.f + ((float)rand() / (float)(RAND_MAX)) * 30.f);

		AttackEffect->SetKirbyOwner(this);
	}

	m_Player->SetAttackEnable(false);

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	m_Player->ChangePlayerNormalIdleAnimation();

}
