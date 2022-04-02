#include "BeamKirbyState.h"
#include <Resource/Mesh/StaticMesh.h>
#include "Scene/Scene.h"
#include "Engine.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Object/Player2D.h"
#include "../Object/KirbyAttackEffect.h"
#include "../Object/BeamUltimate.h"
#include "../Object/BombUltimate.h"

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
{
	m_Player->ChangePlayerAttackAnimation();
}

void CBeamKirbyState::FallDownAttack()
{
	if (m_GoUpState)
		return;

	m_FallAttackState = true;

	m_FallAttackTime = m_FallAttackTimeMax;

	// ������ ���� �ִٸ�
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

		AttackEffect->GetMainSprite()->SetWorldScale(80.f, 80.f, 1.f);

		AttackEffect->SetLeftAttackDir(-1.f);

		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

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

		AttackEffect->SetKirbyOwner(this);
	}
	
	m_Player->SetAttackEnable(false);

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
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

			AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

			AttackEffect->SetRightAttackDir(TraceDir.y);

			AttackEffect->SetAttackDirX(TraceDir.x);

			AttackEffect->SetWorldPos(TargetPos);

			AttackEffect->SetLifeTime(1.5f);

			AttackEffect->SetKirbyOwner(this);
		}
	}
}

void CBeamKirbyState::SpecialAttack()
{
	// ������ ���� �ִٸ�
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	// ���� ����
	if (PlayerMoveDir.x < 0.f)
	{
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetWorldPos(GetWorldPos().x - 100.f, GetWorldPos().y + 500.f, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->AddRelativeRotationZ(90.f);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 4);

		// ���
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack2");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetWorldPos(GetWorldPos().x - 350.f, GetWorldPos().y + 600.f, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->AddRelativeRotationZ(90.f);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 4);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack3");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetWorldPos(GetWorldPos().x - 600.f, GetWorldPos().y + 700.f, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(-1.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->AddRelativeRotationZ(90.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 4);
	}
	// ���������� ���� �ִٸ� --> ������ ����
	else
	{
		// ��
		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetWorldPos(GetWorldPos().x + 100.f, GetWorldPos().y + 500.f, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->AddRelativeRotationZ(-90.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 4);

		// ��� 
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack2");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->SetWorldPos(GetWorldPos().x + 350.f, GetWorldPos().y + 600.f, GetWorldPos().z);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->AddRelativeRotationZ(-90.f);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 4);

		// �Ʒ�
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack3");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::BeamSpecial);
		AttackEffect->SetWorldPos(GetWorldPos().x + 600.f, GetWorldPos().y + 700.f, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(-1.f);
		AttackEffect->SetAttackDirX(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->AddRelativeRotationZ(-90.f);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility() * 4);
	}

	m_Player->SetAttackEnable(false);

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	m_Player->ChangePlayerIdleAnimation();
}

void CBeamKirbyState::UltimateAttack()
{
	float XLeftEnd = -1.f, XRightEnd = -1.f;

	float XDiffStep = -1.f, YDiffStep = -1.f;

	float YStart = -1.f, YEnd = -1.f;

	Resolution RS = CEngine::GetInst()->GetResolution();

	// ������ ���� �־��ٸ�
	if (m_Player->GetObjectMoveDir().x < 0.f)
	{
		// ���� ��
		XLeftEnd = GetWorldPos().x - (float)RS.Width * 0.5f ;
		// ������ ������ �ξ� �� 
		XRightEnd = GetWorldPos().x + (float)RS.Width * 2.5f;
	}
	// ���������� ���� �־��ٸ�
	else
	{
		// ���� ������ �ξ� ��
		XLeftEnd = GetWorldPos().x - (float)RS.Width * 2.5f;
		// ������ ��
		XRightEnd = GetWorldPos().x + (float)RS.Width * 0.5f ;
	}

	XDiffStep = (XRightEnd - XLeftEnd) * 0.05f; // 20�� ������.

	YStart = m_Player->GetWorldPos().y + (float)RS.Height * 5.0f;
	YEnd = m_Player->GetWorldPos().y - (float)RS.Height * 0.5f;

	YDiffStep = (YStart - YEnd) * 0.05f; // 20�� ������.


	for (int row = 0; row < 20; row++)
	{
		for (int col = 0;  col < 20; col++)
		{
			float LittleDiff = ((float)rand() / (float)RAND_MAX) * 150.f;

			bool DiffDir = col & 1 ? -1.f : 1.f;

			CBeamUltimate* AttackEffect = m_Scene->CreateGameObject<CBeamUltimate>("Attack3");

			AttackEffect->SetWorldPos(XLeftEnd + XDiffStep * col + (LittleDiff * DiffDir),
				YEnd + YDiffStep * row + (LittleDiff  * DiffDir),
				GetWorldPos().z);

			AttackEffect->SetKirbyOwner(this);

			AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

			// ������ ���� �־��ٸ�
			if (m_Player->GetObjectMoveDir().x < 0.f)
			{
				AttackEffect->SetLeftAttackDir(-1.f);
				AttackEffect->AddRelativeRotationZ(135.f);
			}
			// ���������� ���� �־��ٸ�
			else
			{
				AttackEffect->SetRightAttackDir(-1.f);
				AttackEffect->AddRelativeRotationZ(-40.f);
			}
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
	// ������ ���� �ִٸ�
	const Vector3& PlayerMoveDir = m_Player->GetObjectMoveDir();

	float TraceRightX = GetWorldPos().x + GetWorldScale().x * 2.f;
	float TraceLeftX = GetWorldPos().x + GetWorldScale().x * 2.f;

	float TraceUpY = GetWorldPos().y + 50.f;
	float TraceDownY = GetWorldPos().y - 50.f;

	// ��
	Vector3 TraceUpDir = {};
	Vector3 TraceDownDir = {};

	// ���� ����
	if (PlayerMoveDir.x < 0.f)
	{
		// ��
		TraceUpDir = Vector3(TraceLeftX, TraceUpY, GetWorldPos().z) - GetWorldPos();
		TraceUpDir.Normalize();

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(TraceUpDir.y);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// ���
		 AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack2");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// �Ʒ�
		TraceDownDir = Vector3(TraceLeftX, TraceDownY, GetWorldPos().z) - GetWorldPos();
		TraceDownDir.Normalize();
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack3");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetLeftAttackDir(TraceDownDir.y);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}
	// ���������� ���� �ִٸ� --> ������ ����
	else
	{
		// ��
		TraceUpDir = Vector3(TraceRightX, TraceUpY, GetWorldPos().z) - GetWorldPos();
		TraceUpDir.Normalize();

		CKirbyAttackEffect* AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack1");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(TraceUpDir.y);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// ��� 
		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack2");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(0.f);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());

		// �Ʒ�
		TraceDownDir = Vector3(TraceRightX, TraceDownY, GetWorldPos().z) - GetWorldPos();
		TraceDownDir.Normalize();

		AttackEffect = m_Scene->CreateGameObject<CKirbyAttackEffect>("Attack3");
		AttackEffect->SetAttackType(KirbyAttackEffect_Type::Beam);
		AttackEffect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * 0.5f,
			GetWorldPos().y, GetWorldPos().z);
		AttackEffect->SetRightAttackDir(TraceDownDir.y);
		AttackEffect->SetKirbyOwner(this);
		AttackEffect->SetAttackDamage(m_ExtraAttackAbility + m_Player->GetAttackAbility());
	}

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	m_Player->ChangePlayerIdleAnimation();

	m_Player->SetAttackEnable(false);
}
