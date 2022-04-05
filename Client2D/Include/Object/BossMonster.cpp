#include "BossMonster.h"
#include <Scene/CameraManager.h>
#include <Scene/Scene.h>
#include <Scene/SceneManager.h>
#include "Engine.h"
#include "Player2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/ColliderCircle.h"
#include "Component/PaperBurnComponent.h"
#include "../UI/BossHUD.h"
#include "BossFightParticle.h"
#include "../Object/EffectRandomStar.h"
#include "../Object/HPRedItem.h"
#include "../Object/HPYellowItem.h"
#include "../Object/HPGreenItem.h"
#include "../ClientManager.h"

CBossMonster::CBossMonster() :
	m_StartBossStage(false),
	m_FarAttackLimitTimeMax(3.5f),
	m_CloseAttackLimitTimeMax(2.5f),
	m_CameraFollowMaxTime(3.f), // 실제 Camera Component의 Max Time
	m_IsRoundStarted(false),
	m_BossAngry(false),
	m_MakeHPItemTimeMax(15.f)
{
	m_MonsterType = Monster_Type::Boss;
	m_AttackAbility = 60.f;
}

CBossMonster::CBossMonster(const CBossMonster& Monster) : CMonster(Monster)
{}

CBossMonster::~CBossMonster()
{}

void CBossMonster::Start()
{
	CMonster::Start();

	m_MonsterType = Monster_Type::Boss;
	m_AttackAbility = 60.f;

	// PaperBurn이 끝나더라도, Destroy 되지 않게 세팅한다.
	m_IsDeathAfterPaperBurn = false;

	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("RightDeath")->SetEndFunction(this, &CBossMonster::StartPaperBurnEffect);
	m_Sprite->GetAnimationInstance()->FindAnimationSequence2DData("LeftDeath")->SetEndFunction(this, &CBossMonster::StartPaperBurnEffect);

	m_PaperBurn->SetFinishCallback(this, &CBossMonster::PaperBurnEndCallback);
}

bool CBossMonster::Init()
{
	if (!CMonster::Init())
		return false;

	return true;
}

void CBossMonster::StartPaperBurnEffect()
{
	m_PaperBurn->StartPaperBurn();
}

void CBossMonster::PaperBurnEndCallback()
{
	m_Scene->SetWorldResolution(m_InitWorldResolution.x, m_InitWorldResolution.y);

	m_Scene->SetBossWorldResolution(0.f, m_InitWorldResolution.y);

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect(true);

	Destroy();
}

void CBossMonster::Damage(float Damage)
{
	CMonster::Damage(Damage); //

	CBossHUD* HUD = dynamic_cast<CBossHUD*>(m_Scene->GetBossHUD());

	HUD->GetHPProgressBar()->SetPercent(m_HP / m_HPMax);
}

void CBossMonster::MakeBossStartEffect()
{
	if (m_StartBossStage)
		return;

	// 1) World 맵 크기 제한
	m_IsRoundStarted = true;

	Resolution RS = CEngine::GetInst()->GetResolution();

	m_InitWorldResolution = m_Scene->GetWorldResolution();

	float WorldRightEnd = GetWorldPos().x + (float)RS.Width * 0.5f;

	if (m_Scene->GetWorldResolution().x <= WorldRightEnd)
	{
		WorldRightEnd = m_Scene->GetWorldResolution().x;
	}

	// m_Scene->SetWorldResolution(WorldRightEnd, m_Scene->GetWorldResolution().y);

	m_Scene->SetBossWorldResolution(WorldRightEnd, m_Scene->GetWorldResolution().y);

	m_Scene->GetCameraManager()->GetCurrentCamera()->ApplyShakeEffect();

	m_StartBossStage = true;

	// 2) 카메라 이동 효과
	CPlayer2D* Player = dynamic_cast<CPlayer2D*>(m_Scene->GetPlayerObject());

	if (!Player)
		return;

	Player->SetCameraFollowMaxTime(m_CameraFollowMaxTime);

	Player->SetCameraFollowBossMonster(this);
}

void CBossMonster::UpdateAttackLimitTimes(float DeltaTime)
{
	if (m_FarAttackLimitTime > 0.f)
	{
		m_FarAttackLimitTime -= DeltaTime;
	}

	if (m_CloseAttackLimitTime > 0.f)
	{
		m_CloseAttackLimitTime -= DeltaTime;
	}
}

void CBossMonster::UpdateBossAngryEffect(float DeltaTime)
{
	if (m_HP < m_HPMax * 0.35f)
	{
		MakeBossAngry();
		// 주기적으로 Particle을 만들어준다.
	}
}

void CBossMonster::MakeBossAngry()
{
	if (m_BossAngry)
		return;

	m_BossAngry = true;

	m_Sprite->SetBaseColor(1.f, 0.3f, 0.3f, 1.f);

	m_AttackAbility *=  1.5f;

	m_MonsterMoveVelocity = m_MonsterMoveVelocity * 2.0f;

	// Particle을 하나 만들어낸다
	MakeBossFightParticleEffect();

	// Random Star Effect
	for (int i = 0; i < 10; i++)
	{
		CEffectRandomStar* RandomStar = m_Scene->CreateGameObject<CEffectRandomStar>("RandomStar");
		RandomStar->SetWorldPos(Vector3(GetWorldPos().x - (GetWorldScale().x * GetPivot().x) * 2.f, GetWorldPos().y, GetWorldPos().z));
	}
}

void CBossMonster::MakeBossFightParticleEffect()
{
	CBossFightParticle* BossFightParticle = m_Scene->CreateGameObject<CBossFightParticle>("BossFightParticle");

	BossFightParticle->SetWorldPos(Vector3(0.f, 0.f, 0.f));
}

void CBossMonster::UpdateMakeHPItemEffect(float DeltaTime)
{
	m_MakeHPItemTime += DeltaTime;

	if (m_MakeHPItemTime >= m_MakeHPItemTimeMax)
	{
		m_MakeHPItemTime = 0.f;


		// 총 5개의 Item 을 주기적으로 만들 것이다.
		for (int i = 0; i < 3; i++)
		{
			float XNumberFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();
			float YNumberFrom0To1 = CClientManager::GetInst()->GenerateRandomNumberFrom0To1();

			int ItemType = (int)(rand()) % 3;

			float XPos =  100 + XNumberFrom0To1 * m_Scene->GetBossWorldResolution().x * 0.9f;
			float YPos = 250.f + YNumberFrom0To1 * m_Scene->GetBossWorldResolution().y * 0.3f;

			CItem* Item = nullptr;

			switch(ItemType)
			{
				case 0:
				{
					Item = m_Scene->CreateGameObject<CHPYellowItem>("Item");
					Item->SetWorldPos(XPos, YPos, 0.f);

					// 그리고 각 Item 위치에 Random Star 들도 만들 것이다
					for (int i = 0; i < 3; i++)
					{
						CEffectRandomStar* Star = m_Scene->CreateGameObject<CEffectRandomStar>("Star");
						Star->SetWorldPos(Item->GetWorldPos());
					}
				}
				break;
				case 1 :
				{
					Item = m_Scene->CreateGameObject<CHPRedItem>("Item");
					Item->SetWorldPos(XPos, YPos, 0.f);

					// 그리고 각 Item 위치에 Random Star 들도 만들 것이다
					for (int i = 0; i < 3; i++)
					{
						CEffectRandomStar* Star = m_Scene->CreateGameObject<CEffectRandomStar>("Star");
						Star->SetWorldPos(Item->GetWorldPos());
					}
				}
				break;
				case 2:
				{
					Item = m_Scene->CreateGameObject<CHPGreenItem>("Item");
					Item->SetWorldPos(XPos, YPos, 0.f);

					// 그리고 각 Item 위치에 Random Star 들도 만들 것이다
					for (int i = 0; i < 3; i++)
					{
						CEffectRandomStar* Star = m_Scene->CreateGameObject<CEffectRandomStar>("Star");
						Star->SetWorldPos(Item->GetWorldPos());
					}
				}
				break;
			}
		}
	}
}

void CBossMonster::AIDeathSpecific(float DeltaTime)
{
	// Boss Monster 를 Stop 시킨다
	m_MonsterMoveVelocity = 0.f;

	// Boss HUD를 점점 사라지게 세팅한다.
	CBossHUD* BossHUD = (CBossHUD*)m_Scene->GetBossHUD();

	BossHUD->StartDestroy();

	// Scene 에서 Boss Fight Particle을 제외한다
	CSceneManager::GetInst()->GetScene()->DeleteGameObjectByType<CBossFightParticle>();
	// m_Scene->DeleteGameObjectByType<CBossFightParticle>();
}

void CBossMonster::Update(float DeltaTime)
{
	if (m_GamePlayDelayTime > 0)
	{
		m_GamePlayDelayTime -= DeltaTime;
		return;
	}

	// 카메라 Boss 따라가기 
	if (m_Scene->GetPlayerObject()->GetWorldPos().x + m_Scene->GetWorldResolution().x > GetWorldPos().x ||
		m_Scene->GetPlayerObject()->GetWorldPos().y + (- 1.f) * m_Scene->GetWorldResolution().y < GetWorldPos().y)
	{
		MakeBossStartEffect();
	}

	if (!m_IsRoundStarted)
		return;

	CMonster::Update(DeltaTime);

	UpdateAttackLimitTimes(DeltaTime);

	UpdateBossAngryEffect(DeltaTime);

	UpdateMakeHPItemEffect(DeltaTime);
}

void CBossMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CBossMonster* CBossMonster::Clone()
{
	return new CBossMonster(*this);
}
