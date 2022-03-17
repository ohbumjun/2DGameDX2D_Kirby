#include "BeamKirbyState.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyNormalAttack.h"

CBeamKirbyState::CBeamKirbyState()
{}

CBeamKirbyState::CBeamKirbyState(const CBeamKirbyState& Kirby) : CKirbyState(Kirby)
{}

CBeamKirbyState::~CBeamKirbyState()
{}

void CBeamKirbyState::Attack()
{}

void CBeamKirbyState::FallDownAttack()
{}

void CBeamKirbyState::GoUpAttack()
{}

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

		CKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack1");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);

		// 가운데
		 AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack2");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(0.f);

		// 아래
		TraceDownDir = Vector3(TraceLeftX, TraceDownY, GetWorldPos().z) - GetWorldPos();
		TraceDownDir.Normalize();
		AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack3");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
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

		CKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack1");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);

		// 가운데 
		AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack2");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);

		// 아래
		TraceDownDir = Vector3(TraceRightX, TraceDownY, GetWorldPos().z) - GetWorldPos();
		TraceDownDir.Normalize();

		AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack3");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
	}

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	m_Player->ChangePlayerIdleAnimation();

	m_Player->SetAttackEnd();
}
