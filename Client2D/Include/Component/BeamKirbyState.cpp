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
	// ������ ���� �ִٸ�
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	if (PlayerMoveDir.x < 0.f)
	{
		// ���
		CKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetLeftAttackDir(0.f);
		// CFightKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CFightKirbyNormalAttack>("Attack");
		// AttackEffect->SetLeftAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
	}
	// ���������� ���� �ִٸ� 
	else
	{
		CKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CKirbyNormalAttack>("Attack");
		AttackEffect->SetAttackType(KirbyNormalAttack_Type::Beam);
		AttackEffect->SetRightAttackDir(0.f);
		// CFightKirbyNormalAttack* AttackEffect = m_Scene->CreateGameObject<CFightKirbyNormalAttack>("Attack");
		// AttackEffect->SetRightAttackDir(0.f);

		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
	}

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	m_Player->ChangePlayerIdleAnimation();

	m_Player->SetAttackEnd();
}
