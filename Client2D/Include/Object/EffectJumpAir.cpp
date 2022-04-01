#include "EffectJumpAir.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "UI/UINumberWidget.h"
#include "UI/UIDamageFont.h"
#include "Monster.h"
#include "Animation/AnimationSequence2DInstance.h"

CEffectJumpAir::CEffectJumpAir() :
	m_MoveDir(1.f),
	m_EffectMoveSpeed(100.f)
{
}

CEffectJumpAir::~CEffectJumpAir()
{
}

void CEffectJumpAir::SetRightEffect()
{
	m_MoveDir = 1.f;

	m_Sprite->GetAnimationInstance()->ChangeAnimation("EffectRight");
}

void CEffectJumpAir::SetLeftEffect()
{
	m_MoveDir = -1.f;

	m_Sprite->GetAnimationInstance()->ChangeAnimation("EffectLeft");
}

bool CEffectJumpAir::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("JumpAirEffect");

	SetRootComponent(m_Sprite);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"JumpAirEffect", TEXT("Object_Jump.anim"));

	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.5f);
	m_Sprite->SetRelativeScale(80.f, 40.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	SetLifeTime(0.7f);

	return true;
}

void CEffectJumpAir::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddWorldPos(Vector3(m_MoveDir, 0.f, 0.f) * m_EffectMoveSpeed * DeltaTime);
}

void CEffectJumpAir::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
