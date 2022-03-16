#include "BeamMonsterAttack.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include <Component/SpriteComponent.h>

CBeamMonsterAttack::CBeamMonsterAttack() :
	m_AttackImageSize(50.f)
{}

CBeamMonsterAttack::CBeamMonsterAttack(const CBeamMonsterAttack& Attack) : CGameObject(Attack)
{}

CBeamMonsterAttack::~CBeamMonsterAttack()
{}

void CBeamMonsterAttack::SetRightAttackDir()
{}

void CBeamMonsterAttack::SetLeftAttackDir()
{}

void CBeamMonsterAttack::Start()
{
	CGameObject::Start();
}

bool CBeamMonsterAttack::Init()
{
	if (!CGameObject::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"BeamAttackEffect", TEXT("Ability_Beam_AttackEffect.anim"));

	float AnimDelayTime = AnimationInstance->GetCurrentAnimation()->GetPlayTime()
	/	AnimationInstance->GetCurrentAnimation()->GetFrameCount();

	m_Root = CreateComponent<CSceneComponent>("Root");

	m_FirstSprite = CreateComponent<CSpriteComponent>("MainSprite");
	m_FirstSprite->SetAnimationInstance(AnimationInstance);
	m_FirstSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_FirstSprite->SetInheritRotZ(true);

	m_SecondSprite = CreateComponent<CSpriteComponent>("SecondSprite");
	m_SecondSprite->SetAnimationInstance(AnimationInstance);
	m_SecondSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_SecondSprite->SetRelativePos(m_AttackImageSize + 5.f, 0.f, 0.f);
	m_SecondSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 2.f);
	m_SecondSprite->SetInheritRotZ(true);

	m_ThirdSprite = CreateComponent<CSpriteComponent>("ThirdSprite");
	m_ThirdSprite->SetAnimationInstance(AnimationInstance);
	m_ThirdSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_ThirdSprite->SetRelativePos((m_AttackImageSize + 5.f) * 2.f, 0.f, 0.f);
	m_ThirdSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 4.f);
	m_ThirdSprite->SetInheritRotZ(true);

	m_FourthSprite = CreateComponent<CSpriteComponent>("FourthSprite");
	m_FourthSprite->SetAnimationInstance(AnimationInstance);
	m_FourthSprite->SetWorldScale(m_AttackImageSize, m_AttackImageSize, 1.f);
	m_FourthSprite->SetRelativePos((m_AttackImageSize + 5.f) * 3.f, 0.f, 0.f);
	m_FourthSprite->GetAnimationInstance()->GetCurrentAnimation()->SetInitPauseTime(AnimDelayTime * 6.f);
	m_FourthSprite->SetInheritRotZ(true);

	m_Root->AddChild(m_FirstSprite);
	m_Root->AddChild(m_SecondSprite);
	m_Root->AddChild(m_ThirdSprite);
	m_Root->AddChild(m_FourthSprite);

	return true;
}

void CBeamMonsterAttack::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_Root->AddRelativeRotationZ(200.f * DeltaTime);
}
