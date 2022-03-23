#include "EffectWaterBlast.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "UI/UINumberWidget.h"
#include "UI/UIDamageFont.h"
#include "Monster.h"
#include "Animation/AnimationSequence2DInstance.h"

CEffectWaterBlast::CEffectWaterBlast() 
{
}

CEffectWaterBlast::~CEffectWaterBlast()
{
}

bool CEffectWaterBlast::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("WaterBlastEffect");

	SetRootComponent(m_Sprite);

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"WaterBlastEffect", TEXT("Item_WaterBlast.anim"));

	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetPlayTime(0.5f);
	m_Sprite->SetRelativeScale(140.f, 90.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	SetLifeTime(0.5f);

	return true;
}

void CEffectWaterBlast::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectWaterBlast::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}
