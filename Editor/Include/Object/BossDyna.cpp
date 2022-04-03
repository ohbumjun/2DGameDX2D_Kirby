#include "BossDyna.h"

#include <Component/ColliderCircle.h>

#include "Animation/AnimationSequence2DInstance.h"
#include "UI/MonsterEditorHUD.h"
#include "Component/WidgetComponent.h"
#include "Component/SpriteComponent.h"

class CAnimationSequence2DInstance;

CBossDyna::CBossDyna()
{
	SetTypeID<CBossDyna>();
}


CBossDyna::~CBossDyna()
{}

void CBossDyna::Start()
{
	CBossMonster::Start();

	CWidgetComponent* Component = FindComponentByType<CWidgetComponent>();

	if (Component)
	{
		CMonsterEditorHUD* MonsterHUD = dynamic_cast<CMonsterEditorHUD*>(Component->GetWidgetWindow());

		if (MonsterHUD)
		{
			MonsterHUD->SetText(TEXT("Dyna")); //
		}
	}
}

bool CBossDyna::Init()
{
	if (!CBossMonster::Init())
		return false;

	LoadAnimationInstance("Dyna", TEXT("Boss_Dyna_Body.anim"));
	SetCurrentAnimation("RightIdle");

	SetWorldScale(350.f, 350.f, 1.f);

	m_ColliderBody->SetInfo(Vector2(0.f, 0.f), m_RootComponent->GetWorldScale().x * 0.4f);

	m_ColliderBody->SetRelativePos(0.f, 50.f, 0.f);


	m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(6.f);

	// Head
	m_DynaHead = CreateComponent<CSpriteComponent>("DynaHead");

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"DynaHead", TEXT("Boss_Dyna_Head.anim"), ENGINE_ANIMATION_PATH);

	m_DynaHead->SetAnimationInstance(AnimationInstance);

	m_DynaHead->GetAnimationInstance()->SetCurrentAnimation("RightIdle");

	// Right Foot
	m_DynaRightFoot = CreateComponent<CSpriteComponent>("DynaRightFoot");

	AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"DynaFoot", TEXT("Boss_Dyna_Foot.anim"), ENGINE_ANIMATION_PATH);

	m_DynaRightFoot->SetAnimationInstance(AnimationInstance);

	m_DynaRightFoot->GetAnimationInstance()->SetCurrentAnimation("RightIdle");

	m_DynaRightFoot->SetWorldScale(50.f, 50.f, 1.f);

	// Left Foot
	m_DynaLeftFoot = CreateComponent<CSpriteComponent>("DynaLeftFoot");

	AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"DynaFoot", TEXT("Boss_Dyna_Foot.anim"), ENGINE_ANIMATION_PATH);

	m_DynaLeftFoot->SetAnimationInstance(AnimationInstance);

	m_DynaLeftFoot->GetAnimationInstance()->SetCurrentAnimation("RightIdle");

	m_DynaLeftFoot->SetWorldScale(50.f, 50.f, 1.f);

	m_RootComponent->AddChild(m_DynaHead);
	m_RootComponent->AddChild(m_DynaRightFoot);
	m_RootComponent->AddChild(m_DynaLeftFoot);

	// Left
	// m_DynaHead->SetRelativePos(-80.f, m_RootComponent->GetWorldScale().y * 0.25f, 0.f);
	// m_DynaRightFoot->SetRelativePos(-15.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
	// m_DynaLeftFoot->SetRelativePos(-115.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);

	return true;
}

void CBossDyna::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	const std::string CurrentAnim = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetName();

	if (CurrentAnim == "LeftIdle")
	{
		int CurFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

		if (CurFrame == 0) // Âß »¸±â
		{
			SetWorldScale(700.f, 350.f, 1.f);
		}
		else if (CurFrame == 2)
		{
			SetWorldScale(450.f, 350.f, 1.f); // Áß°£ »¸±â
		}
		else // ¾Æ·¡·Î 
		{
			SetWorldScale(350.f, 350.f, 1.f);
		}

		if (m_DynaHead)
			m_DynaHead->SetRelativePos(-80.f, m_RootComponent->GetWorldScale().y * 0.28f, 0.f);
		if (m_DynaLeftFoot)
			m_DynaLeftFoot->SetRelativePos(-115.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
		if (m_DynaRightFoot)
			m_DynaRightFoot->SetRelativePos(-15.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
	}
	else if (CurrentAnim == "RightIdle")
	{
		int CurFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

		if (CurFrame == 0) // Âß »¸±â
		{
			SetWorldScale(700.f, 350.f, 1.f);
		}
		else if (CurFrame == 2)
		{
			SetWorldScale(450.f, 350.f, 1.f); // Áß°£ »¸±â
		}
		else // ¾Æ·¡·Î 
		{
			SetWorldScale(350.f, 350.f, 1.f);
		}

		if (m_DynaHead)
			m_DynaHead->SetRelativePos(-10.f, m_RootComponent->GetWorldScale().y * 0.28f, 0.f);
		if (m_DynaRightFoot)
			m_DynaRightFoot->SetRelativePos(65.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
		if (m_DynaLeftFoot)
			m_DynaLeftFoot->SetRelativePos(-35.f, m_RootComponent->GetWorldScale().y * 0.12f * -1.f, 0.f);
	}
}

void CBossDyna::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}
