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
	// m_CameraFollowMaxTime = 10.f; // 처음에는 무한정 세팅하고 --> Apper 가 마무리 되면 그때 Player로
	m_CameraFollowMaxTime = FLT_MAX; // 처음에는 무한정 세팅하고 --> Apper 가 마무리 되면 그때 Player로
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

	// Trace를 별도로 하지 않는다
	m_IsTracingMonster = false;

	// Collider 세팅
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

	// 처음에는 LeftBlackAppearance 로 시작
	
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("LeftBlackAppear");

	// 크기도 작게 + 이후 점점 크게 할 것
	SetWorldScale(10.f, 10.f, 1.f);

	m_ScaleIncreasing = 10.f;

	// Collider Body는 처음 등장 때는 동작하지 않게 세팅한다
	m_ColliderBody->Enable(false);

	// 처음에는 오른쪽에서 왼쪽으로 이동하게 세팅해야 한다.
	SetWorldPos(m_Scene->GetWorldResolution().x + 200.f, m_Scene->GetWorldResolution().y * 0.2f, 0.f);

	// 몸을 제외한 나머지 부위는 안보이게 세팅
	m_DynaRightFoot->GetMaterial()->SetOpacity(0.f);
	m_DynaLeftFoot->GetMaterial()->SetOpacity(0.f);
	m_DynaHead->GetMaterial()->SetOpacity(0.f);

	// 초반에는 Resolution 범위 밖을 벗어나게끔 허용한다
	m_ApplyLimitPosResolution = false;

	// 처음에는 저 화면 뒤쪽에 그린 느낌을 위해 Layer 를 뒤쪽에 있는 Tile 로 세팅
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

	// Head 의 경우, 위 아래로 이동시키기
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
	// 중복 방지
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

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
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

	// 머리, 발은 안보이게 세팅하고
	m_DynaHead->Enable(false);
	m_DynaRightFoot->Enable(false);
	m_DynaLeftFoot->Enable(false);

	// 모든 Boss Dyna Baby는 제거한다.
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
			// Scene Change 를 진행하는 Object를 만들어낸다.
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

	// Frame 에 따른 크기 조정
	if (m_ObjectMoveDir.x < 0.f)
	{
		int CurFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

		if (CurFrame == 0) // 쭉 뻗기
		{
			SetWorldScale(700.f, 350.f, 1.f);
		}
		else if (CurFrame == 2)
		{
			SetWorldScale(450.f, 350.f, 1.f); // 중간 뻗기
		}
		else // 아래로 
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

		if (CurFrame == 0) // 쭉 뻗기
		{
			SetWorldScale(700.f, 350.f, 1.f);
		}
		else if (CurFrame == 2)
		{
			SetWorldScale(450.f, 350.f, 1.f); // 중간 뻗기
		}
		else // 아래로 
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

	// 좌우 이동
	if (m_MovementRight)
	{
		AddWorldPos(Vector3(1.f, 0.f, 0.f) * DeltaTime * m_MonsterMoveVelocity);

		if (GetWorldPos().x >= m_MovementTargetXPos)
		{
			m_MovementRight = false;

			m_MovementTargetXPos = GetWorldPos().x - GetWorldScale().x * 0.5f - ((float)rand() / (float)RAND_MAX) * 1500.f;

			// 최대 크기가 700.f 로 세팅되어 있다
			// GetWorldScale 로 하면, Frame마다 크기가 달라서, 화면에서 확 이동하게 되는 이상한 느낌이 날 것이다.
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

	// 상하 이동
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

	// 크기를 점점 증가시킨다.
	m_ScaleIncreasing += 20.f * DeltaTime;

	if (m_ScaleIncreasing >= 350.f)
	{
		m_ScaleIncreasing = 350.f;
	}

	// 흐른 시간을 누적시킨다.
	m_AppearFlowTime += DeltaTime;

	SetWorldScale(m_ScaleIncreasing * 2.0f, m_ScaleIncreasing, 1.f);

	if (m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName() == "LeftBlackAppear")
	{
		// 왼쪽으로 이동하게 세팅한다.
		AddWorldPos(Vector3(-1.f, 0.12f, 0.f) * DeltaTime * 500.f);

		if (GetWorldPos().x <= -100.f)
		{
			m_Sprite->GetAnimationInstance()->ChangeAnimation("RightBlackAppear");
		}
	}
	else if (m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName() == "RightBlackAppear")
	{
		// 서서히 오른쪽으로 이동하게 세팅한다.
		AddWorldPos(Vector3(1.f, 0.12f, 0.f) * DeltaTime * 500.f);

		if (GetWorldPos().x >= m_Scene->GetWorldResolution().x)
		{
			m_Sprite->GetAnimationInstance()->ChangeAnimation("AppearFront");

			/// SetWorldPos(m_Scene->GetWorldResolution().x, GetWorldPos().y, 0.f);
			SetWorldPos(1300.f, GetWorldPos().y, 0.f);

			// 이후 점점 왼쪽으로 오게 세팅한다.
			m_MovementRight = false;

			m_MovementTargetXPos = 400.f;

			// 다시 World Resolution 범위 안에 제한되도록 세팅한다.
			m_ApplyLimitPosResolution = true;
		}
	}
	else if (m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName() == "AppearFront")
	{
		// 해당 위치에서 점점 Scale만 커지게 세팅한다.
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

			// 다시 Collider Body가 동작하게 세팅한다.
			m_ColliderBody->Enable(true);

			// 혹시 모르니, 머리의 상대적 위치를 다시 세팅한다.
			m_DynaHead->SetRelativePos(-80.f, m_InitHeadYRelativePos, 0.f);

			m_Scene->GetCameraManager()->GetCurrentCamera()->CancleTargetBossFollow();

			// 다시 Head, Foot들을 보이게 세팅한다.
			m_DynaHead->GetMaterial()->SetOpacity(1.f);
			m_DynaRightFoot->GetMaterial()->SetOpacity(1.f);
			m_DynaLeftFoot->GetMaterial()->SetOpacity(1.f);

			// 다시 원래의 Layer 로 세팅
			m_Sprite->SetLayerName("Default");

			// Boss HUD가 등장하게 세팅한다
			m_Scene->GetViewPort()->StartBossHUDCallback();
		}
	}
}

