#include "BossDyna.h"
#include <Component/SpriteComponent.h>
#include <Scene/CameraManager.h>
#include "Component/PaperBurnComponent.h"
#include <Scene/Scene.h>
#include "Scene/SceneManager.h"
#include "Engine.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "DynaFarAttack.h"
#include "DynaCloseAttack.h"
#include "Component/ColliderCircle.h"
#include "EffectSceneChangeAlpha.h"
#include "BossDynaBaby.h"
#include "DynaNest.h"
#include "../Scene/EndingScene.h"

class CAnimationSequence2DInstance;

CBossDyna::CBossDyna() :
	m_SceneChangeLimitTime(-1.f),
	m_HeadToggleMaxTime(0.5f),
	m_MakeDynaNestTime(15.f)
{
	SetTypeID<CBossDyna>();

	// m_DashDistance = 1000.f;
	// m_JumpVelocity = 50.f;
	// m_IsGroundObject = false;
	// m_PhysicsSimulate = false;
	// m_AttackDistance = 1100.f;
	// m_CloseAttackDistance = 550.f;
	// m_FarAttackDistance = 1100.f;
	// m_FarAttackLimitTime = 0.f;
	// m_FarAttackLimitTimeMax = 3.f;
	// m_HitLimitTimeMax = 0.4f;
	// m_HP = 5000.f;
	// m_HPMax = 5000.f;

	m_HitLimitTimeMax = 0.4f;
	m_FarAttackLimitTime = 0.f;
	// m_CameraFollowMaxTime = 10.f; // ó������ ������ �����ϰ� --> Apper �� ������ �Ǹ� �׶� Player��
	m_CameraFollowMaxTime = FLT_MAX; // ó������ ������ �����ϰ� --> Apper �� ������ �Ǹ� �׶� Player��
}

CBossDyna::~CBossDyna()
{}

void CBossDyna::Start()
{
	CBossMonster::Start();

	Monster_Stat* Stat = SetExcelStat(L"BossDyna");
	m_FarAttackDistance = Stat->m_FarAttackDist;
	m_CloseAttackDistance = Stat->m_CloseAttackDist;

	m_IsGround = false;

	m_Sprite->GetAnimationInstance()->Play();

	m_MovementTargetYPos = GetWorldPos().y;

	// m_HP = 5000.f;d
	m_HP = 40.f;
	// m_HPMax = 5000.f;
	// m_HPMax = 200.f;

	// Trace�� ������ ���� �ʴ´�
	m_IsTracingMonster = false;

	// Collider ����
	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_RootComponent->GetWorldScale().x * 0.35f);

	m_ColliderBody->SetRelativePos(0.f, 100.f, 0.f);

	// Head
	m_DynaHead = (CSpriteComponent*)FindComponent("DynaHead");

	m_DynaHead->SetWorldScale(130.f, 130.f, 1.f);

	m_InitHeadYRelativePos = m_DynaHead->GetRelativePos().y;

	m_DynaHead->SetRelativePos(-80.f, m_InitHeadYRelativePos, 0.f);

	// Right Foot
	m_DynaRightFoot = (CSpriteComponent*)FindComponent("DynaRightFoot");

	m_DynaRightFoot->SetWorldScale(50.f, 50.f, 1.f);

	// Left Foot
	m_DynaLeftFoot = (CSpriteComponent*)FindComponent("DynaLeftFoot");

	m_DynaLeftFoot->SetWorldScale(50.f, 50.f, 1.f);


	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightDeath")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftDeath")->SetLoop(false);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightBlackAppear")->SetLoop(true);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftBlackAppear")->SetLoop(true);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetPlayTime(2.0f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
	this, &CBossDyna::CloseAttack);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetPlayTime(2.0f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CBossDyna::CloseAttack);
	
	m_IsAttacking = false;

	// ó������ LeftBlackAppearance �� ����
	
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("LeftBlackAppear");

	// ũ�⵵ �۰� + ���� ���� ũ�� �� ��
	SetWorldScale(10.f, 10.f, 1.f);

	m_ScaleIncreasing = 10.f;

	// Collider Body�� ó�� ���� ���� �������� �ʰ� �����Ѵ�
	m_ColliderBody->Enable(false);

	// ó������ �����ʿ��� �������� �̵��ϰ� �����ؾ� �Ѵ�.
	SetWorldPos(m_Scene->GetWorldResolution().x + 200.f, m_Scene->GetWorldResolution().y * 0.2f, 0.f);

	// ���� ������ ������ ������ �Ⱥ��̰� ����
	m_DynaRightFoot->GetMaterial()->SetOpacity(0.f);
	m_DynaLeftFoot->GetMaterial()->SetOpacity(0.f);
	m_DynaHead->GetMaterial()->SetOpacity(0.f);

	// �ʹݿ��� Resolution ���� ���� ����Բ� ����Ѵ�
	m_ApplyLimitPosResolution = false;

	// ó������ �� ȭ�� ���ʿ� �׸� ������ ���� Layer �� ���ʿ� �ִ� Tile �� ����
	m_Sprite->SetLayerName("Tile");
	
	m_IsAppearing = true;
}

bool CBossDyna::Init()
{
	if (!CBossMonster::Init())
		return false;

	return true;
}

void CBossDyna::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	UpdateSceneChangeLimitTime(DeltaTime);

	UpdateScale(DeltaTime);

	// Head �� ���, �� �Ʒ��� �̵���Ű��
	UpdateHeadToggle(DeltaTime);

	if (!m_IsAppearing)
	{
		UpdateMovement(DeltaTime);
	}

	UpdateAppearance(DeltaTime);

	UpdateMakeNestTime(DeltaTime);

}

void CBossDyna::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

void CBossDyna::AddBossDynaBaby(CBossDynaBaby* Baby)
{
	// �ߺ� ����
	auto iter = std::find(m_BabiesList.begin(), m_BabiesList.end(), Baby);

	if (iter != m_BabiesList.end())
		return;

	m_BabiesList.push_back(Baby);
}

void CBossDyna::DeleteDynaBaby(CBossDynaBaby* Baby)
{
	auto iter = m_BabiesList.begin();
	auto iterEnd = m_BabiesList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Baby)
		{
			m_BabiesList.erase(iter);
			return;
		}
	}
}

void CBossDyna::FarAttack()
{
	m_IsAttacking = true;

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();

	for (int i = 0; i < 5; i++)
	{
		float XAddPos = ((float)(rand()) / (float)(RAND_MAX)) * (float)CEngine::GetInst()->GetResolution().Width;
		float YPos = (float)CEngine::GetInst()->GetResolution().Height + 300.f;

		CDynaFarAttack* AttackEffect = m_Scene->CreateGameObject<CDynaFarAttack>("Attack");

		AttackEffect->SetMonsterOwner(this);
		// AttackEffect->SetPhysicsSimulate(true);

		if (m_ObjectMoveDir.x < 0.f)
		{
			AttackEffect->m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectLeft");
			AttackEffect->SetLeftAttackDir(-1.f);
			AttackEffect->SetWorldPos(600.f + XAddPos, YPos, GetWorldPos().z);
		}
		else
		{
			AttackEffect->m_MainSprite->GetAnimationInstance()->SetCurrentAnimation("EffectRight");
			AttackEffect->SetRightAttackDir(-1.f);
			AttackEffect->SetWorldPos(-600.f + XAddPos, YPos, GetWorldPos().z);
		}
	}

	m_IsAttacking = false;

	// ���������� �վ��� ������ ���� �����ϱ� ���Ͽ� Animation�� �ѹ� �ٲ��ش�.
	ChangeIdleAnimation();
}

void CBossDyna::CloseAttack()
{
	m_IsAttacking = true;

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();

	for (int i = 0; i < 3; i++)
	{
		float Number0To1 = (float)rand() / (float)RAND_MAX;

		// Attack Back Effect
		CDynaCloseAttack* AttackEffect = m_Scene->CreateGameObject<CDynaCloseAttack>("AttackEffect");

		float XPos = -1.f;

		if (m_ObjectMoveDir.x < 0.f)
		{
			AttackEffect->SetLeftAttackDir();

			XPos = GetWorldPos().x - Number0To1 * 300.f;
		}
		else
		{
			AttackEffect->SetRightAttackDir();

			XPos = GetWorldPos().x + Number0To1 * 300.f;
		}

		AttackEffect->SetWorldPos(XPos, GetWorldPos().y - 200.f + 400.f * Number0To1, GetWorldPos().z);

		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetLifeTime(1.3f);
		
	}

	m_IsAttacking = false;

	ChangeIdleAnimation();
}

void CBossDyna::AIAttackSpecific(float DeltaTime)
{
	if (m_IsAppearing)
		return;

	if (m_ObjectMoveDir.x < 0.f)
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("LeftAttack");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("LeftAttack");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("LeftAttack");
	}
	else
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("RightAttack");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("RightAttack");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("RightAttack");
	}

	if (m_IsAttacking)
		return;

	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());

	if (DistToPlayer > m_CloseAttackDistance && DistToPlayer < m_FarAttackDistance)
	{
		if (m_FarAttackLimitTime > 0.f)
			return;

		m_FarAttackLimitTime = m_FarAttackLimitTimeMax;

		ChangeFarAttackAnimation();
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

		ChangeCloseAttackAnimation();
	}

}

void CBossDyna::AITraceSpecific(float DeltaTime)
{
	if (m_ObjectMoveDir.x < 0.f)
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("LeftRun");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("LeftRun");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("LeftRun");
	}
	else
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("RightRun");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("RightRun");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("RightRun");
	}
}

void CBossDyna::AIWalkSpecific(float DeltaTime)
{
	CBossMonster::AIWalkSpecific(DeltaTime);

	if (m_ObjectMoveDir.x < 0.f)
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("LeftWalk");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("LeftWalk");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("LeftWalk");
	}
	else
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("RightWalk");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("RightWalk");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("RightWalk");
	}
}

void CBossDyna::AIIdleSpecific(float DeltaTime)
{
	CBossMonster::AIIdleSpecific(DeltaTime);

	if (m_ObjectMoveDir.x < 0.f)
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("LeftIdle");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("LeftIdle");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("LeftIdle");
	}
	else
	{
		m_DynaHead->GetAnimationInstance()->ChangeAnimation("RightIdle");
		m_DynaRightFoot->GetAnimationInstance()->ChangeAnimation("RightIdle");
		m_DynaLeftFoot->GetAnimationInstance()->ChangeAnimation("RightIdle");
	}
}

void CBossDyna::AIHitSpecific(float DeltaTime)
{
	CBossMonster::AIHitSpecific(DeltaTime);

	m_DynaHead->GetMaterial()->SetOpacity(0.f);
	m_DynaRightFoot->GetMaterial()->SetOpacity(0.f);
	m_DynaLeftFoot->GetMaterial()->SetOpacity(0.f);

	m_YPosBeforeHit = GetWorldPos().y;

	SetWorldPos(GetWorldPos().x, GetWorldPos().y + 50.f, 1.f);
}

void CBossDyna::AIDeathSpecific(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
		return;

	CBossMonster::AIDeathSpecific(DeltaTime);

	// �Ӹ�, ���� �Ⱥ��̰� �����ϰ�
	m_DynaHead->Enable(false);
	m_DynaRightFoot->Enable(false);
	m_DynaLeftFoot->Enable(false);

	// ��� Boss Dyna Baby�� �����Ѵ�.
	auto iter = m_BabiesList.begin();
	auto iterEnd = m_BabiesList.end();

	for (; iter != iterEnd;)
	{
		(*iter)->m_HP = -1.f;
		iter = m_BabiesList.erase(iter);
	}

	m_SceneChangeLimitTime = 5.f;
}

void CBossDyna::ChangeFarAttackAnimation()
{
	if (m_IsAppearing)
		return;

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CBossDyna::FarAttack);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CBossDyna::FarAttack);

	m_IsAttacking = true;
	
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttack");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttack");
}

void CBossDyna::ChangeCloseAttackAnimation()
{
	if (m_IsAppearing)
		return;

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttack")->SetEndFunction(
		this, &CBossDyna::CloseAttack);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttack")->SetEndFunction(
		this, &CBossDyna::CloseAttack);

	m_IsAttacking = true;

	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttack");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttack");
}

void CBossDyna::UpdateSceneChangeLimitTime(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
	{
		m_SceneChangeLimitTime -= DeltaTime;

		if (m_SceneChangeLimitTime <= 0.f)
		{
			// Scene Change �� �����ϴ� Object�� ������.
			CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

			Alpha->SetSceneStart(false);

			Alpha->SetSceneChangeCallback(this, &CBossDyna::ChangeSceneToEndingScene);
		}
	}
}

void CBossDyna::ChangeSceneToEndingScene()
{
	CSceneManager::GetInst()->CreateNewScene();
	CSceneManager::GetInst()->CreateSceneMode<CEndingScene>(false);

	if (m_Scene->GetPlayerHUD() && m_Scene->GetPlayerHUD()->IsEnable())
	{
		m_Scene->GetPlayerHUD()->Enable(false);
	}

	Destroy();
}

void CBossDyna::MakeBossAngry()
{
	CBossMonster::MakeBossAngry();

	m_DynaHead->SetBaseColor(1.f, 0.3f, 0.3f, 1.f);
	m_DynaRightFoot->SetBaseColor(1.f, 0.3f, 0.3f, 1.f);
	m_DynaLeftFoot->SetBaseColor(1.f, 0.3f, 0.3f, 1.f);
}

void CBossDyna::UpdateMakeNestTime(float DeltaTime)
{
	if (m_AI == Monster_AI::Death)
	{
		m_MakeDynaNestFlowTime = 0.f;
		return;
	}

	if (m_IsAppearing)
		return;

	m_MakeDynaNestFlowTime += DeltaTime;

	if (m_MakeDynaNestFlowTime >= m_MakeDynaNestTime)
	{
		m_MakeDynaNestFlowTime = 0.f;

		CDynaNest* DynaNest = m_Scene->CreateGameObject<CDynaNest>("DynaNest");

		DynaNest->SetWorldPos(GetWorldPos().x, GetWorldPos().y - GetWorldScale().y * GetPivot().y, GetWorldPos().z);

		DynaNest->m_BossDyna = this;
	}
}

void CBossDyna::ChangeTraceAnimation()
{
	if (m_IsAppearing)
		return;
	CBossMonster::ChangeTraceAnimation();
}

void CBossDyna::ChangeAttackAnimation()
{
	if (m_IsAppearing)
		return;
	CBossMonster::ChangeAttackAnimation();
}

void CBossDyna::ChangeWalkAnimation()
{
	if (m_IsAppearing)
		return;
	CBossMonster::ChangeWalkAnimation();
}

void CBossDyna::ChangeIdleAnimation()
{
	if (m_IsAppearing)
		return;
	CBossMonster::ChangeIdleAnimation();
}

void CBossDyna::SpecificActionAfterHitTimeEnd()
{
	if (m_IsAppearing)
		return;

	m_DynaHead->GetMaterial()->SetOpacity(1.f);
	m_DynaRightFoot->GetMaterial()->SetOpacity(1.f);
	m_DynaLeftFoot->GetMaterial()->SetOpacity(1.f);

	SetWorldPos(GetWorldPos().x, m_YPosBeforeHit, GetWorldPos().z);
}

void CBossDyna::UpdateScale(float DeltaTime)
{
	if (m_IsAppearing)
		return;

	// Frame �� ���� ũ�� ����
	if (m_ObjectMoveDir.x < 0.f)
	{
		int CurFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

		if (CurFrame == 0) // �� ����
		{
			SetWorldScale(700.f, 350.f, 1.f);
		}
		else if (CurFrame == 2)
		{
			SetWorldScale(450.f, 350.f, 1.f); // �߰� ����
		}
		else // �Ʒ��� 
		{
			SetWorldScale(350.f, 350.f, 1.f);
		}

		if (m_DynaHead)
			m_DynaHead->SetRelativePos(-120.f, m_DynaHead->GetRelativePos().y, 0.f);
		if (m_DynaLeftFoot)
			m_DynaLeftFoot->SetRelativePos(-115.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
		if (m_DynaRightFoot)
			m_DynaRightFoot->SetRelativePos(-15.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
	}
	else
	{
		int CurFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

		if (CurFrame == 0) // �� ����
		{
			SetWorldScale(700.f, 350.f, 1.f);
		}
		else if (CurFrame == 2)
		{
			SetWorldScale(450.f, 350.f, 1.f); // �߰� ����
		}
		else // �Ʒ��� 
		{
			SetWorldScale(350.f, 350.f, 1.f);
		}

		if (m_DynaHead)
			m_DynaHead->SetRelativePos(-10.f, m_DynaHead->GetRelativePos().y, 0.f);
		if (m_DynaRightFoot)
			m_DynaRightFoot->SetRelativePos(65.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
		if (m_DynaLeftFoot)
			m_DynaLeftFoot->SetRelativePos(-35.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
	}
}

void CBossDyna::UpdateHeadToggle(float DeltaTime)
{
	if (m_IsAppearing)
		return;

	m_HeadToggleTime += DeltaTime;

	float CurYRelativePos = m_DynaHead->GetRelativePos().y;
	float CurXRelativePos = m_DynaHead->GetRelativePos().x;

	if (m_HeadToggleUp)
	{
		m_DynaHead->SetRelativePos(CurXRelativePos, CurYRelativePos + 20.f * DeltaTime, 0.f);

		if (m_HeadToggleTime >= m_HeadToggleMaxTime)
		{
			m_HeadToggleTime = 0.f;

			m_HeadToggleUp = false;
		}
	}
	else
	{
		m_DynaHead->SetRelativePos(CurXRelativePos, CurYRelativePos - 20.f * DeltaTime, 0.f);

		if (m_HeadToggleTime >= m_HeadToggleMaxTime)
		{
			m_HeadToggleTime = 0.f;

			m_HeadToggleUp = true;
		}
	}
}

void CBossDyna::UpdateMovement(float DeltaTime)
{
	if (m_IsDead)
		return;

	// �¿� �̵�
	if (m_MovementRight)
	{
		AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (GetWorldPos().x >= m_MovementTargetXPos)
		{
			m_MovementRight = false;

			m_MovementTargetXPos = GetWorldPos().x - GetWorldScale().x * 0.5f - ((float)rand() / (float)RAND_MAX) * 1500.f;

			// �ִ� ũ�Ⱑ 700.f �� ���õǾ� �ִ�
			// GetWorldScale �� �ϸ�, Frame���� ũ�Ⱑ �޶�, ȭ�鿡�� Ȯ �̵��ϰ� �Ǵ� �̻��� ������ �� ���̴�.
			if (m_MovementTargetXPos <= 370.f)
			{
				m_MovementTargetXPos = 370.f;
			}
		}
	}
	else
	{
		AddWorldPos(Vector3(-1.f, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (GetWorldPos().x <= m_MovementTargetXPos)
		{
			m_MovementRight = true;

			m_MovementTargetXPos = GetWorldPos().x + GetWorldScale().x * 0.5f + ((float)rand() / (float)RAND_MAX) * 1500.f;

			if (m_MovementTargetXPos >= m_Scene->GetBossWorldResolution().x - 370.f)
			{
				m_MovementTargetXPos = m_Scene->GetBossWorldResolution().x - 370.f;
			}
		}
	}

	// ���� �̵�
	if (m_MovementUp)
	{
		AddWorldPos(Vector3(0.f, 1.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (GetWorldPos().y + GetWorldScale().y * 0.5f >= m_MovementTargetYPos)
		{
			m_MovementUp = false;

			m_MovementTargetYPos = GetWorldPos().y - GetWorldScale().y * 0.5f - ((float)rand() / (float)RAND_MAX) * 1000.f;

			if (m_MovementTargetYPos <= GetWorldScale().y)
			{
				m_MovementTargetYPos = GetWorldScale().y + 0.001f;;
			}
		}
	}
	else
	{
		AddWorldPos(Vector3(0.f, -1.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (GetWorldPos().y - GetWorldScale().y * 0.5f <= m_MovementTargetYPos)
		{
			m_MovementUp = true;

			m_MovementTargetYPos = GetWorldPos().y + GetWorldScale().y * 0.5f + ((float)rand() / (float)RAND_MAX) * 1000.f;

			if (m_MovementTargetYPos >= m_Scene->GetBossWorldResolution().y - GetWorldScale().y * 0.7f)
			{
				m_MovementTargetYPos = m_Scene->GetBossWorldResolution().y - GetWorldScale().y * 0.7f;
			}
		}
	}
}

void CBossDyna::UpdateAppearance(float DeltaTime)
{
	if (!m_IsAppearing)
		return;

	// ũ�⸦ ���� ������Ų��.
	m_ScaleIncreasing += 20.f * DeltaTime;

	if (m_ScaleIncreasing >= 350.f)
	{
		m_ScaleIncreasing = 350.f;
	}

	// �帥 �ð��� ������Ų��.
	m_AppearFlowTime += DeltaTime;

	SetWorldScale(m_ScaleIncreasing * 2.0f, m_ScaleIncreasing, 1.f);

	if (m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName() == "LeftBlackAppear")
	{
		// �������� �̵��ϰ� �����Ѵ�.
		AddWorldPos(Vector3(-1.f, 0.12f, 0.f) * DeltaTime * 500.f);

		if (GetWorldPos().x <= -100.f)
		{
			m_Sprite->GetAnimationInstance()->ChangeAnimation("RightBlackAppear");
		}
	}
	else if (m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName() == "RightBlackAppear")
	{
		// ������ ���������� �̵��ϰ� �����Ѵ�.
		AddWorldPos(Vector3(1.f, 0.12f, 0.f) * DeltaTime * 500.f);

		if (GetWorldPos().x >= m_Scene->GetWorldResolution().x)
		{
			m_Sprite->GetAnimationInstance()->ChangeAnimation("AppearFront");

			/// SetWorldPos(m_Scene->GetWorldResolution().x, GetWorldPos().y, 0.f);
			SetWorldPos(1300.f, GetWorldPos().y, 0.f);

			// ���� ���� �������� ���� �����Ѵ�.
			m_MovementRight = false;

			m_MovementTargetXPos = 400.f;

			// �ٽ� World Resolution ���� �ȿ� ���ѵǵ��� �����Ѵ�.
			m_ApplyLimitPosResolution = true;
		}
	}
	else if (m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName() == "AppearFront")
	{
		// �ش� ��ġ���� ���� Scale�� Ŀ���� �����Ѵ�.
		UpdateMovement(DeltaTime);

		if (m_AppearFlowTime >= 15.f)
		{
			m_CameraFollowMaxTime = m_AppearFlowTime;

			SetWorldScale(350.f, 350.f, 350.f);

			if (m_Scene->GetPlayerObject()->GetWorldPos().x  < GetWorldPos().x)
			{
				m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftIdle");
			}
			else
			{
				m_Sprite->GetAnimationInstance()->ChangeAnimation("RightIdle");
			}

			m_IsAppearing = false;

			// �ٽ� Collider Body�� �����ϰ� �����Ѵ�.
			m_ColliderBody->Enable(true);

			// Ȥ�� �𸣴�, �Ӹ��� ����� ��ġ�� �ٽ� �����Ѵ�.
			m_DynaHead->SetRelativePos(-80.f, m_InitHeadYRelativePos, 0.f);

			m_Scene->GetCameraManager()->GetCurrentCamera()->CancleTargetBossFollow();

			// �ٽ� Head, Foot���� ���̰� �����Ѵ�.
			m_DynaHead->GetMaterial()->SetOpacity(1.f);
			m_DynaRightFoot->GetMaterial()->SetOpacity(1.f);
			m_DynaLeftFoot->GetMaterial()->SetOpacity(1.f);

			// �ٽ� ������ Layer �� ����
			m_Sprite->SetLayerName("Default");

			// Boss HUD�� �����ϰ� �����Ѵ�
			m_Scene->GetViewPort()->StartBossHUDCallback();
		}
	}
}

