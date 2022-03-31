#include "BossTree.h"
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

CBossTree::CBossTree()  :
	m_SceneChangeLimitTime(-1.f)
{
	SetTypeID<CBossTree>();

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

CBossTree::CBossTree(const CBossTree& Monster) : CBossMonster(Monster)
{}

CBossTree::~CBossTree()
{}

void CBossTree::Start()
{
	CBossMonster::Start();

	Monster_Stat* Stat = SetExcelStat(L"BossTree");
	m_FarAttackDistance = Stat->m_FarAttackDist;
	m_CloseAttackDistance = Stat->m_CloseAttackDist;
	m_MonsterMoveVelocity = 0.f;


	m_IsGround = false;


	m_Sprite->GetAnimationInstance()->Play();

	// m_HP = 5000.f;
	m_HP = 50.f;
	// m_HPMax = 5000.f;
	m_HPMax = 50.f;

	// Close Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackClose")->SetEndFunction(this, &CBossTree::CloseAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackClose")->SetEndFunction(this, &CBossTree::CloseAttack);

	// Far Attack
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightAttackFar")->SetEndFunction(this, &CBossTree::FarAttack);

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetPlayTime(1.3f);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetLoop(false);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftAttackFar")->SetEndFunction(this, &CBossTree::FarAttack);

	// Hit 이후에는, 바로 Idle 로 넘어가게 세팅한다.
	/*
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftHit")->SetEndFunction(this,
		&CMonster::ChangeIdleAnimation);
		*/

	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_RootComponent->GetWorldScale().x * 0.7f);

	m_IsAttacking = false;
}

bool CBossTree::Init()
{
	if (!CBossMonster::Init())
		return false;

	return true;
}

void CBossTree::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);


	m_ObjectMoveDir = Vector3(-1.f, 0.f, 0.f);

	UpdateSceneChangeLimitTime(DeltaTime);
}

void CBossTree::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CBossTree* CBossTree::Clone()
{
	return new CBossTree(*this);
}

void CBossTree::FarAttack()
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

void CBossTree::CloseAttack()
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

void CBossTree::AIAttackSpecific(float DeltaTime)
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
	}
	else if (DistToPlayer <= m_CloseAttackDistance)
	{
		if (m_CloseAttackLimitTime > 0.f)
			return;

		m_CloseAttackLimitTime = m_CloseAttackLimitTimeMax;

		ChangeCloseAttackAnimation();
	}
}

void CBossTree::AITraceSpecific(float DeltaTime)
{
	float DistToPlayer = m_Scene->GetPlayerObject()->GetWorldPos().Distance(GetWorldPos());
}

void CBossTree::AIDeathSpecific(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
		return;

	CBossMonster::AIDeathSpecific(DeltaTime);

	m_SceneChangeLimitTime = 5.f;
}

void CBossTree::ChangeFarAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackFar");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackFar");
}

void CBossTree::ChangeCloseAttackAnimation()
{
	if (m_ObjectMoveDir.x < 0.f)
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LeftAttackClose");
	else
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RightAttackClose");
}

void CBossTree::UpdateSceneChangeLimitTime(float DeltaTime)
{
	if (m_SceneChangeLimitTime > 0.f)
	{
		m_SceneChangeLimitTime -= DeltaTime;

		if (m_SceneChangeLimitTime <= 0.f)
		{
			// Scene Change 를 진행하는 Object를 만들어낸다.
			CEffectSceneChangeAlpha* Alpha = m_Scene->CreateGameObject<CEffectSceneChangeAlpha>("Alpha");

			Alpha->SetSceneStart(false);

			Alpha->SetSceneChangeCallback(this, &CBossTree::ChangeSceneToFloat1Scene);
		}
	}
}

void CBossTree::ChangeSceneToFloat1Scene()
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


void CBossTree::ChangeTraceAnimation()
{
	if (m_Jump)
		return;

	CBossMonster::ChangeTraceAnimation();
}

