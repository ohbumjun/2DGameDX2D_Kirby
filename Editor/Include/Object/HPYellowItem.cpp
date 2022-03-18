#include "HPYellowItem.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CHPYellowItem::CHPYellowItem()
{
	m_ItemType = Item_Type::HPYellowPotion;
	SetTypeID<CHPYellowItem>();
}

CHPYellowItem::CHPYellowItem(const CHPYellowItem& Monster)
{}

CHPYellowItem::~CHPYellowItem()
{}

void CHPYellowItem::Start()
{
	CItem::Start();
}

bool CHPYellowItem::Init()
{
	if (!CItem::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"YellowItem", TEXT("Item_HP_Yellow.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("ItemRight");

	SetWorldScale(50.f, 50.f, 1.f);

	return true;
}

void CHPYellowItem::Update(float DeltaTime)
{
	CItem::Update(DeltaTime);
}

void CHPYellowItem::PostUpdate(float DeltaTime)
{
	CItem::PostUpdate(DeltaTime);
}

CHPYellowItem* CHPYellowItem::Clone()
{
	return new CHPYellowItem(*this);
}

void  CHPYellowItem::Save(FILE* pFile)
{
	CItem::Save(pFile);
}