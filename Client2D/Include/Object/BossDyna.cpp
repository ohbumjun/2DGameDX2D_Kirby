#include "BossDyna.h"
#include "Tornado.h"
#include <Component/SpriteComponent.h>
#include <Scene/CameraManager.h>
#include "Component/PaperBurnComponent.h"
#include <Scene/Scene.h>
#include "Scene/SceneManager.h"
#include "Engine.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Apple.h"
#include "Component/ColliderCircle.h"
#include "EffectSceneChangeAlpha.h"
#include "../Scene/Float1Scene.h"
#include "../Scene/Float1LoadingScene.h"

class CAnimationSequence2DInstance;

CBossDyna::CBossDyna() :
	m_SceneChangeLimitTime(-1.f),
	m_HeadToggleMaxTime(0.5f)
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
	m_CameraFollowMaxTime = 7.f;
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
	m_MonsterMoveVelocity = 0.f;

	m_Sprite->GetAnimationInstance()->Play();

	// m_HP = 5000.f;
	m_HP = 50.f;
	// m_HPMax = 5000.f;
	m_HPMax = 50.f;


	// Collider 세팅
	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_RootComponent->GetWorldScale().x * 0.3f);

	m_ColliderBody->SetRelativePos(0.f, 100.f, 0.f);

	// Head
	m_DynaHead = (CSpriteComponent*)FindComponent("DynaHead");

	m_DynaHead->SetWorldScale(110.f, 110.f, 1.f);

	m_DynaHead->SetRelativePos(-80.f, m_RootComponent->GetWorldScale().y * 0.f, 0.f);

	// Right Foot
	m_DynaRightFoot = (CSpriteComponent*)FindComponent("DynaRightFoot");

	m_DynaRightFoot->SetWorldScale(50.f, 50.f, 1.f);

	// Left Foot
	m_DynaLeftFoot = (CSpriteComponent*)FindComponent("DynaLeftFoot");

	m_DynaLeftFoot->SetWorldScale(50.f, 50.f, 1.f);

	// Close Attack
	/*
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CBossDyna::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CBossDyna::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CBossDyna::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CBossDyna::FarAttack);
	*/


	m_IsAttacking = false;
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
			m_DynaHead->SetRelativePos(-80.f, m_DynaHead->GetRelativePos().y, 0.f);
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

	// Head 의 경우, 위 아래로 이동시키기
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

void CBossDyna::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

void CBossDyna::FarAttack()
{
	m_IsAttacking = true;

	for (int i = 0; i < 3; i++)
	{
		float XPos = ((float)(rand()) / (float)(RAND_MAX)) * (float)CEngine::GetInst()->GetResolution().Width;
		float YPos = (float)CEngine::GetInst()->GetResolution().Height;

		CApple* AttackEffect = m_Scene->CreateGameObject<CApple>("Attack");

		AttackEffect->SetWorldPos(XPos, YPos, GetWorldPos().z);
		AttackEffect->SetMonsterOwner(this);
		AttackEffect->SetPhysicsSimulate(true);
	}

	m_IsAttacking = false;

	// 연속적으로 뿜어져 나오는 것을 방지하기 위하여 Animation을 한번 바꿔준다.
	ChangeIdleAnimation();
}

void CBossDyna::CloseAttack()
{
	m_IsAttacking = true;

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();

	// Attack Back Effect
	CTornado* AttackEffect = m_Scene->CreateGameObject<CTornado>("AttackEffect");

	AttackEffect->SetWorldPos(
		GetWorldPos().x + GetWorldScale().x * 0.3f,
		GetWorldPos().y - GetWorldScale().y * 0.4f,
		0.f);

	AttackEffect->SetLeftAttackDir();
	AttackEffect->SetMonsterOwner(this);
	AttackEffect->SetLifeTime(3.0f);

	m_IsAttacking = false;

	ChangeIdleAnimation();
}

void CBossDyna::AIAttackSpecific(float DeltaTime)
{
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

	/*
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
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

		ChangeCloseAttackAnimation();
	}
	*/

}

void CBossDyna::AITraceSpecific(float DeltaTime)
{
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
}

void CBossDyna::AIWalkSpecific(float DeltaTime)
{
	CBossMonster::AIWalkSpecific(DeltaTime);

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
}

void CBossDyna::AIIdleSpecific(float DeltaTime)
{
	CBossMonster::AIIdleSpecific(DeltaTime);

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
}

void CBossDyna::AIDeathSpecific(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
		return;

	CBossMonster::AIDeathSpecific(DeltaTime);

	m_SceneChangeLimitTime = 5.f;
}

void CBossDyna::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CBossDyna::ChangeCloseAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
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

			Alpha->SetSceneChangeCallback(this, &CBossDyna::ChangeSceneToFloat1Scene);
		}
	}
}

void CBossDyna::ChangeSceneToFloat1Scene()
{
	Destroy();

	CSceneManager::GetInst()->CreateNewScene();
	CSceneManager::GetInst()->CreateSceneMode<CFloat1LoadingScene>(false);

	/*
	CSceneManager::GetInst()->CreateNewScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CFloat1Scene>(false);
	CSceneManager::GetInst()->GetNextScene()->PrepareResources();
	if (CSceneManager::GetInst()->GetNextScene()->Load("Float1.scn", SCENE_PATH))
	{
		CSceneManager::GetInst()->ChangeNextScene();
	}
	*/
}


void CBossDyna::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

