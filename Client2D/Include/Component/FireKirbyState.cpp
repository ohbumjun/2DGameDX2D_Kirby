#include "FireKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyNormalAttack.h"
#include "../Object/FireAttackBackEffect.h"

class CFightMonsterAttack;

CFireKirbyState::CFireKirbyState() :
	m_GoUpTimeMax(0.4f),
	m_GoUpTime(0.f),
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_GoUpState(false)
{}

CFireKirbyState::CFireKirbyState(const CFireKirbyState& Kirby) : CKirbyState(Kirby)
{}

CFireKirbyState::~CFireKirbyState()
{}

void CFireKirbyState::Attack()
{
}

void CFireKirbyState::FallDownAttack()
{}

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

void CFireKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpTime > 0.f)
	{
		m_GoUpTime -= DeltaTime;

		SetWorldScale(Vector3(m_InitWorldScale.x * 2.f, m_InitWorldScale.y * 2.f, m_InitWorldScale.z));

		if (m_Player->GetObjectMoveDir().x > 0)
		{
			AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * 900.f);
		}
		else
		{
			AddWorldPos(Vector3(1.0f * -1.f, 0.f, 0.f) * DeltaTime * 900.f);
		}

		if (m_GoUpTime <= 0.f)
		{
			m_GoUpState = false;

			SetWorldScale(m_InitWorldScale);
		}
	}
}

void CFireKirbyState::UpdateFallAttack(float DeltaTime)
{}

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
	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// 가운데
		CKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(0.f);
	}
	// 오른쪽으로 보고 있다면 
	else
	{
		CKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack");
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(0.f);
	}
	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();
}
