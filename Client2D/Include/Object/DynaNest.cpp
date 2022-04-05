#include "DynaNest.h"

#include "BossDyna.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "BossDynaBaby.h"
#include "Scene/CameraManager.h"

CDynaNest::CDynaNest() :
	m_GenerateDynaBabyTime(2.f)
{}

CDynaNest::~CDynaNest()
{}

void CDynaNest::MakeDynaBabyEffect()
{
	float NumberFrom0To1 = (float)rand() / (float)RAND_MAX;

	// 오른쪽 2개
	for (int i = 0; i < 2; i++)
	{
		float XPos = i & 1 ? GetWorldPos().x + NumberFrom0To1 * 100.f : GetWorldPos().x - NumberFrom0To1 * 100.f;

		CBossDynaBaby* DynaBaby = m_Scene->CreateGameObject<CBossDynaBaby>("Attack");

		DynaBaby->SetWorldPos(XPos, GetWorldPos().y, 0.f);

		DynaBaby->m_ObjectMoveDir.x = i & 1 ? 1.f : -1.f;
		// AttackEffect->SetRightAttackDir(0.f);

		DynaBaby->m_JumpVelocity = 40.f + NumberFrom0To1 * 10.f;

		DynaBaby->JumpStart();

		DynaBaby->SetWorldScale(70.f, 70.f, 1.f);

		DynaBaby->m_BossDyna = m_BossDyna;

		// Boss Dyna 의 Baby List 에 추가 
		m_BossDyna->AddBossDynaBaby(DynaBaby);
	}
}

void CDynaNest::Start()
{
	CGameObject::Start();
}

bool CDynaNest::Init()
{
	if (!CGameObject::Init())
		return false;

	m_NestSprite = CreateComponent<CSpriteComponent>("MonsterSprite");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance("DynaNest", TEXT("Boss_Dyna_NEst.anim"), ENGINE_ANIMATION_PATH);

	// Clone 해서 세팅해줘야 한다.
	m_NestSprite->SetAnimationInstance(AnimationInstance);

	SetWorldScale(200.f, 120.f, 1.f);

	return true;
}

void CDynaNest::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// 계속 아래로 떨어진다.
	AddWorldPos(Vector3(0.f, -1.f, 0.f) * DeltaTime * 100.f);

	m_TimeFlowed += DeltaTime;

	// Dyna Baby 들을 만들어낸다.
	if (m_TimeFlowed >= m_GenerateDynaBabyTime)
	{
		MakeDynaBabyEffect();

		Destroy();
	}
}

void CDynaNest::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
