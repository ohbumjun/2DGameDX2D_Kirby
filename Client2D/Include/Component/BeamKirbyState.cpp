#include "BeamKirbyState.h"

#include <Resource/Mesh/StaticMesh.h>

#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"
#include "../Object/FireAttackBackEffect.h"

CBeamKirbyState::CBeamKirbyState() :
	m_GoUpTimeMax(1.5f),
	m_GoUpTime(0.f),
	m_FallAttackTime(0.f),
	m_FallAttackTimeMax(0.4f),
	m_FallAttackState(false),
	m_GoUpState(false)
{
	m_ExtraAttackAbility = 5.f;
}

CBeamKirbyState::CBeamKirbyState(const CBeamKirbyState& Kirby) : CKirbyState(Kirby)
{}

CBeamKirbyState::~CBeamKirbyState()
{}

void CBeamKirbyState::Attack()
{}

void CBeamKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();
	
	for (int i = 0; i < 8; i++)
	{
		Vector3 TargetPos = {};
		Vector3 TraceDir = {};

		TargetPos.x = GetWorldPos().x + cosf(DegreeToRadian(45 * (float)i)) * (150.f);
		TargetPos.y = GetWorldPos().y + sinf(DegreeToRadian(45 * (float)i)) * (150.f);

		TraceDir = TargetPos - GetWorldPos();

		TraceDir.Normalize();

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");

		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);

		AttackEffect->SetWorldScale(60.f, 60.f, 1.f);

		AttackEffect->SetLeftAttackDir(-1.f);

		// AttackEffect->SetOwner(this);

		AttackEffect->SetWorldPos(TargetPos);

		AttackEffect->SetLifeTime(1.5f);

		AttackEffect->SetAttackDirX(TraceDir.x);

		if (PlayerMoveDir.x < 0.f)
		{
			AttackEffect->GetRootComponent()->SetRelativeRotationZ(45.f);

			AttackEffect->SetAttackDirX(-1.f);
		}
		else
		{
			AttackEffect->GetRootComponent()->SetRelativeRotationZ(45.f * -1.f);

			AttackEffect->SetAttackDirX(1.f);
		}
	}
	
	m_Player->SetAttackEnable(false);

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerFallAnimation();
}

void CBeamKirbyState::GoUpAttack()
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

	for (int i = 0; i < 12.f; i++)
	{
		Vector3 TargetPos = {};
		Vector3 TraceDir = {};

		float Angle = -1.f;

		for (int j = 1; j <= 4; j++)
		{
			if (j & 1)
				Angle = 30.f * i;
			else
				Angle = 30.f * i + 15.f;

			TargetPos.x = GetWorldPos().x + cosf(DegreeToRadian(Angle)) * (35.f * j);
			TargetPos.y = GetWorldPos().y + sinf(DegreeToRadian(Angle)) * (35.f * j);

			TraceDir = TargetPos - GetWorldPos();

			TraceDir.Normalize();

			CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack");
			AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpark);

			AttackEffect->SetRightAttackDir(TraceDir.y);

			AttackEffect->SetAttackDirX(TraceDir.x);

			AttackEffect->SetWorldPos(TargetPos);

			AttackEffect->SetLifeTime(1.5f);
		}

	}
}

void CBeamKirbyState::UpdateAttackGoUpState(float DeltaTime)
{
	if (m_GoUpTime > 0.f)
	{
		m_GoUpTime -= DeltaTime;

		m_Player->SetAttackEnable(true);

		if (m_GoUpTime <= 0.f)
		{
			m_GoUpState = false;

			m_Player->ChangePlayerFallAnimation();

			m_Player->SetAttackEnable(false);

			m_Player->SetPhysicsSimulate(true);
		}
	}
}

void CBeamKirbyState::UpdateFallAttack(float DeltaTime)
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

void CBeamKirbyState::Start()
{
	CKirbyState::Start();

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);
}

bool CBeamKirbyState::Init()
{
	if (!CKirbyState::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("Kirby_Beam", TEXT("Kirby_Beam.anim"));

	SetAnimationInstance(AnimationInstance);

	m_Animation->FindAnimationSequence2DData("RightJump")->SetLoop(false);
	m_Animation->FindAnimationSequence2DData("LeftJump")->SetLoop(false);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetPlayTime(0.3f);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(0.3f);

	m_Animation->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CBeamKirbyState::NormalAttackCallback);
	m_Animation->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CBeamKirbyState::NormalAttackCallback);

	return true;
}

void CBeamKirbyState::Update(float DeltaTime)
{
	CKirbyState::Update(DeltaTime);

	UpdateAttackGoUpState(DeltaTime);

	UpdateFallAttack(DeltaTime);
}

void CBeamKirbyState::PostUpdate(float DeltaTime)
{
	CKirbyState::PostUpdate(DeltaTime);
}

void CBeamKirbyState::PrevRender()
{
	CKirbyState::PrevRender();
}

void CBeamKirbyState::Render()
{
	CKirbyState::Render();
}

void CBeamKirbyState::PostRender()
{
	CKirbyState::PostRender();
}

CBeamKirbyState* CBeamKirbyState::Clone()
{
	return new CBeamKirbyState(*this);
}

void CBeamKirbyState::NormalAttackCallback()
{
	// 왼쪽을 보고 있다면
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	float TraceRightX = GetWorldPos().x + GetWorldScale().x * 2.f;
	float TraceLeftX = GetWorldPos().x + GetWorldScale().x * 2.f;

	float TraceUpY = GetWorldPos().y + 50.f;
	float TraceDownY = GetWorldPos().y - 50.f;

	// 위
	Vector3 TraceUpDir = {};
	Vector3 TraceDownDir = {};

	// 왼쪽 공격
	if (PlayerMoveDir.x < 0.f)
	{
		// 위
		TraceUpDir = Vector3(TraceLeftX, TraceUpY, GetWorldPos().z) - GetWorldPos();
		TraceUpDir.Normalize();

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);

		// 가운데
		 AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack2");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(0.f);

		// 아래
		TraceDownDir = Vector3(TraceLeftX, TraceDownY, GetWorldPos().z) - GetWorldPos();
		TraceDownDir.Normalize();
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack3");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(TraceDownDir.y);
	}
	// 오른쪽으로 보고 있다면 --> 오른쪽 공격
	else
	{
		// 위
		TraceUpDir = Vector3(TraceRightX, TraceUpY, GetWorldPos().z) - GetWorldPos();
		TraceUpDir.Normalize();

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);

		// 가운데 
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack2");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		// 아래
		TraceDownDir = Vector3(TraceRightX, TraceDownY, GetWorldPos().z) - GetWorldPos();
		TraceDownDir.Normalize();

		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack3");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
	}

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

	m_Player->SetAttackEnable(false);
}
