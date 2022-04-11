#include "BossPenguin.h"
#include <Component/SpriteComponent.h>
#include <Scene/CameraManager.h>
#include <Scene/Scene.h>
#include "Animation/AnimationSequence2DInstance.h"
#include "PenguinCloseAttack.h"
#include "Component/ColliderCircle.h"
#include "EffectJumpAir.h"
#include "EffectSceneChangeAlpha.h"
#include "../Scene/Dyna1LoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

class CAnimationSequence2DInstance;

CBossPenguin::CBossPenguin() :
	m_JumpEnable(false),
	m_JumpLimitTimeMax(5.f),
	m_JumpDistance(1200.f),
	m_DashRunDistance(700.f),
	m_FarAttackTimeMax(4.5f),
	m_FarAttackTime(-1.f),
	m_SceneChangeLimitTime(-1.f)
{
	SetTypeID<CBossPenguin>();
	m_CameraFollowMaxTime = 1.f;
}

CBossPenguin::~CBossPenguin()
{}

void CBossPenguin::Start()
{
	CBossMonster::Start();
	
	Monster_Stat* Stat = SetExcelStat(L"BossPenguin");
	m_FarAttackDistance = Stat->m_FarAttackDist;
	m_CloseAttackDistance = Stat->m_CloseAttackDist;

	m_IsGround = true;

	m_Sprite->GetAnimationInstance()->Play();

	// Close Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(0.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CBossPenguin::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(0.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CBossPenguin::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(4.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetLoop(false);
	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CBossPenguin::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(4.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetLoop(false);
	// m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CBossPenguin::FarAttack);

	// Jump
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightJump")->SetPlayTime(1.5f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftJump")->SetPlayTime(1.5f);

	// Collider Body Collision Callback 적용하기
	m_ColliderBody->AddCollisionCallback(Collision_State::Begin, (CMonster*)this, &CBossPenguin::OnMonsterBodyCollisionBegin);

	m_InitMoveVelocity = m_MonsterMoveVelocity;

	m_InitJumpAccel = m_JumpAccel;

	m_IsAttacking = false;
}

bool CBossPenguin::Init()
{
	if (!CBossMonster::Init())
		return false;

	return true;
}

void CBossPenguin::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	UpdateJumpAction(DeltaTime);

	UpdateFarAttackAction(DeltaTime);

	UpdateSceneChangeLimitTime(DeltaTime);
}

void CBossPenguin::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

void CBossPenguin::JumpStart()
{
	m_Jump = true;
	m_IsGround = false;
	m_FallTime = 0.f;
	m_FallStartY = GetWorldPos().y;
	m_JumpStart = true;
	m_JumpVelocity = 115.f;
	m_JumpLimitTime = m_JumpLimitTimeMax;
	m_MonsterMoveVelocity = 200.f;
	m_JumpAccel = 1.5f;
	ChangeJumpAttackAnimation();
	m_InitTraceDir = m_TraceDir;

	m_Scene->GetResource()->SoundPlay("ScissorStartJump");

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();
}

void CBossPenguin::FarAttack()
{
	m_FarAttackTime = m_FarAttackTimeMax;

	m_IsAttacking = true;

	// Jump
	m_Jump = true;
	m_IsGround = false;
	m_FallTime = 0.f;
	m_FallStartY = GetWorldPos().y;
	m_JumpVelocity = 90.f;
	m_JumpLimitTime = m_JumpLimitTimeMax;
	
	m_MonsterMoveVelocity = 400.f;

	m_FarAttackTraceDir = m_Scene->GetPlayerObject()->GetWorldPos() - GetWorldPos();

	m_FarAttackTraceDir.Normalize();

	MakeJumpAirEffect();

	m_Scene->GetResource()->SoundPlay("BossFarAttack");
}

void CBossPenguin::FarAttackEnd()
{
	m_FarAttackTime = -1.f;

	m_IsAttacking = false;

	m_MonsterMoveVelocity = m_InitMoveVelocity;
}

void CBossPenguin::CloseAttack()
{
	m_IsAttacking = true;

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();

	// Attack Back Effect
	CPenguinCloseAttack* AttackEffect = m_Scene->CreateGameObject<CPenguinCloseAttack>("AttackEffect");
	//
	if (m_ObjectMoveDir.x < 0.f)
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x - GetWorldScale().x * 0.7f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);

		AttackEffect->SetLeftAttackDir();
	}
	else
	{
		AttackEffect->SetWorldPos(
			GetWorldPos().x + GetWorldScale().x * 0.7f,
			GetWorldPos().y - GetWorldScale().y * 0.3f,
			0.f);

		AttackEffect->SetRightAttackDir();
	}

	AttackEffect->SetMonsterOwner(this);

	AttackEffect->AddRelativeRotationZ(90.f);

	AttackEffect->SetLifeTime(2.f);

	m_IsAttacking = false;

	ChangeIdleAnimation();

	m_Scene->GetResource()->SoundPlay("BossCloseAttack");
}

void CBossPenguin::AIAttackSpecific(float DeltaTime)
{
	if (m_Jump)
		return;

	if (m_IsAttacking)
		return;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer < m_FarAttackDistance)
	{
		if (m_FarAttackLimitTime > 0.f)
			return;

		m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

		ChangeFarAttackAnimation();

		FarAttack();

		// 이동안은, 상대 Player 에게 돌진하게 한다.
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

		ChangeCloseAttackAnimation();

		// 원거리 점프 이후, 계속 이동 중이었다면  --> FarAttack을 종료시켜 준다.
		if (m_FarAttackTime > 0.f)
		{
			FarAttackEnd();
		}
	}
}

void CBossPenguin::AITraceSpecific(float DeltaTime)
{
	if (m_IsAttacking)
		return;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_DashRunDistance && DistToPlayer <= m_JumpDistance)
	{
		if (m_IsBottomCollided && m_JumpLimitTime <= 0.f)
		{
			JumpStart();

			MakeJumpAirEffect();
		}
	}
	else if (DistToPlayer < m_DashRunDistance)
	{
		// 그냥 계속 그대로 진행 (ChangeTraceAnimation)
	}
}

void CBossPenguin::AIDeathSpecific(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
		return;

	CBossMonster::AIDeathSpecific(DeltaTime);

	m_SceneChangeLimitTime = 5.f;
}

void CBossPenguin::AIHitSpecific(float DeltaTime)
{
	m_MonsterMoveVelocity = m_InitMoveVelocity;
}

void CBossPenguin::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CBossPenguin::ChangeCloseAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
}

void CBossPenguin::ChangeJumpAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftJump");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightJump");
}

void CBossPenguin::UpdateJumpAction(float DeltaTime)
{
	// 설령 점프 중이 아니라고 하더라도,
	// Trace Dir 만큼 계속 이동 시킨다.
	if (m_Jump && !m_IsAttacking)
	{
		AddWorldPos(Vector3(m_InitTraceDir.x, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);
	}

	if (m_JumpLimitTime >= 0.f)
	{
		m_JumpLimitTime -= DeltaTime;
	}
}

void CBossPenguin::UpdateFarAttackAction(float DeltaTime)
{
	if (m_FarAttackTime > 0.f)
	{
		m_IsAttacking = true;

		m_FarAttackTime -= DeltaTime;

		AddWorldPos(Vector3(m_FarAttackTraceDir.x, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (m_FarAttackTime < 0.01f)
		{
			FarAttackEnd();
		}
	}
}

void CBossPenguin::MakeJumpAirEffect()
{
	CEffectJumpAir* Effect = m_Scene->CreateGameObject<CEffectJumpAir>("JumpAir");
	Effect->SetRightEffect();
	Effect->SetWorldPos(GetWorldPos().x + GetWorldScale().x * GetPivot().x, GetWorldPos().y - GetPivot().y * GetWorldScale().y, GetWorldPos().z);

	Effect = m_Scene->CreateGameObject<CEffectJumpAir>("JumpAir");
	Effect->SetLeftEffect();
	Effect->SetWorldPos(GetWorldPos().x - GetWorldScale().x * GetPivot().x, GetWorldPos().y - GetPivot().y * GetWorldScale().y, GetWorldPos().z);
}

void CBossPenguin::SetObjectLand()
{
	CBossMonster::SetObjectLand();
	
	m_Jump = false;

	m_JumpAccel = m_InitJumpAccel;
}

void CBossPenguin::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	if (m_IsAttacking)
		return;

	CBossMonster::ChangeTraceAnimation();
}

void CBossPenguin::ChangeSceneToDyna1Scene()
{
	m_Scene->GetResource()->SoundPlay("PlayerChangeSceneStart");

	Destroy();

	CSceneManager::GetInst()->CreateNewScene();
	CSceneManager::GetInst()->CreateSceneMode<CDyna1LoadingScene>(false);
}


void CBossPenguin::UpdateSceneChangeLimitTime(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
	{
		m_SceneChangeLimitTime -= DeltaTime;

		if (m_SceneChangeLimitTime <= 0.f)
		{
			// Scene Change 를 진행하는 Object를 만들어낸다.
			CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

			Alpha->SetSceneStart(false);

			Alpha->SetSceneChangeCallback(this, &CBossPenguin::ChangeSceneToDyna1Scene);
		}
	}
}
