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
	m_CreateBossDynaFunction();

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
