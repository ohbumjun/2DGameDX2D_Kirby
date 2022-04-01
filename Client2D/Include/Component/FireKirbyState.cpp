#include "FireKirbyState.h"
#include "Component/ColliderCircle.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"
#include "../Object/FireAttackBackEffect.h"
#include "Component/ColliderBox2D.h"

class CFightMonsterAttack;

CFireKirbyState::CFireKirbyState() :
	m_GoUpTimeMax(0.4f),
	m_GoUpTime(0.f),
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_IsSpecialAttack(false),
	m_GoUpState(false)
{
	m_ExtraAttackAbility = 5.f;
}

CFireKirbyState::CFireKirbyState(const CFireKirbyState& Kirby) : CKirbyState(Kirby)
{}

CFireKirbyState::~CFireKirbyState()
{}

void CFireKirbyState::Attack()
{
	m_Player->ChangePlayerAttackAnimation();
}

void CFireKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	// 총 10개의 Object를 만들어낸다.
	float XLeftEnd = GetWorldPos().x - (GetWorldScale().x * 0.8f) * 8.f;

	float XStepSize = (GetWorldScale().x * 0.8f);

	for (int i = 0; i < 16; i++)
	{
		// Attack Effect
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::FireFall);

		AttackEffect->SetRightAttackDir(-1.f); // 해당 Animation은 EffectRight 만이 존재한다.

		AttackEffect->SetWorldPos(XLeftEnd + XStepSize * i,
			GetWorldPos().y, GetWorldPos().z);

		AttackEffect->SetAttackDirX(0.f);

		AttackEffect->SetBottomCollisionEnable(true);

		AttackEffect->SetKirbyOwner(this);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// Attack Back Effect
		CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

		BackEffect->SetWorldPos(XLeftEnd + XStepSize * i,
			GetWorldPos().y, GetWorldPos().z);

		BackEffect->SetWorldScale(80.f, 80.f, 1.f);

		BackEffect->AddRelativeRotationZ(-90.f);

		BackEffect->SetLifeTime(0.3f);

		BackEffect->GetColliderBody()->SetInfo(Vector2(0.f, 0.f), BackEffect->GetWorldScale().x * 0.5f);

		BackEffect->GetColliderBody()->SetCollisionProfile("PlayerAttack");
	}
}

void CFireKirbyState::GoUpAttack()
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

	CFireAttackBackEffect* BackEffect = m_Scene->CreateGameObject<CFireAttackBackEffect>("BackFire");

	BackEffect->SetWorldPos(GetWorldPos());
	BackEffect->SetWorldScale(150.f, 150.f, 1.f);
	BackEffect->SetLifeTime(0.3f);


	// 왼쪽
	if (m_Player->GetObjectMoveDir().x < 0)
		BackEffect->SetLeftAttackDir();
	// 오른쪽
	else
		BackEffect->SetRightAttackDir();
}

void CFireKirbyState::SpecialAttack()
{
	m_IsSpecialAttack = true;


}

void CFireKirbyState::UltimateAttack()
{}

void CFireKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpTime > 0.f)
	{
		m_GoUpTime -= DeltaTime;

		m_Player->SetAttackEnable(true);

		SetWorldScale(Vector3(m_InitWorldScale.x * 3.f, m_InitWorldScale.y * 3.f, m_InitWorldScale.z));

		if (m_Player->GetObjectMoveDir().x > 0)
			AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * 900.f);
		else
			AddWorldPos(Vector3(1.0f * -1.f, 0.f, 0.f) * DeltaTime * 900.f);

		m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x * 2.3f, m_InitColliderLength.y * 1.3f);

		if (m_GoUpTime <= 0.f)
		{
			m_GoUpState = false;

			SetWorldScale(m_InitWorldScale);

			m_Player->ChangePlayerFallAnimation();

			m_Player->SetAttackEnable(false);

			m_Player->GetBodyCollider()->SetExtend(m_InitColliderLength.x, m_InitColliderLength.y);
		}
	}
}

void CFireKirbyState::UpdateFallAttack(float DeltaTime)
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

			m_Player->ChangePlayerFallAnimation();
		}
	}
}

void CFireKirbyState::UpdateSpecialAttack(float DeltaTime)
{
	if (!m_IsSpecialAttack)
		return;

	m_SpecialAttackTime += DeltaTime;

	// 총 1초가 걸리게 세팅한다.
	if (m_SpecialAttackTime >= 0.2f)
	{
		m_SpecialAttackTime = 0.f;

		CreateAttackEffect();

		m_NumSpecialAttack += 1;

		if (m_NumSpecialAttack >= 5)
		{
			m_NumSpecialAttack = 0;

			m_SpecialAttackTime = 0.f;

			m_IsSpecialAttack = false;

			m_Player->SetAttackEnd();

			m_Player->ChangePlayerIdleAnimation();
		}
	}
}

void CFireKirbyState::CreateAttackEffect()
{
	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("NormalFireAttack");
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
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}
}

void CFireKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CFireKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Fire", TEXT("Kirby_Fire.anim"));

	SetAnimationInstance(AnimationInstance);

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightFallAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftFallAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CFireKirbyState::NormalAttackCallback);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CFireKirbyState::NormalAttackCallback);

	return true;
}

void CFireKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);

	UpdateAttackGoUpState(DeltaTime);

	UpdateFallAttack(DeltaTime);

	UpdateSpecialAttack(DeltaTime);
}

void CFireKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CFireKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CFireKirbyState::Render()
{
	CKirbyState::Render();
}

void CFireKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

CFireKirbyState* CFireKirbyState::Clone()
{
	return new CFireKirbyState(*this);
}

void CFireKirbyState::NormalAttackCallback()
{
	CreateAttackEffect();
	
	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();
}
