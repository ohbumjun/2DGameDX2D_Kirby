#include "HPGreenItem.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CHPGreenItem::CHPGreenItem()
{
	m_ItemType = Item_Type::HPGreenPotion;
	SetTypeID<CHPGreenItem>();
}

CHPGreenItem::CHPGreenItem(const CHPGreenItem& Monster)
{}

CHPGreenItem::~CHPGreenItem()
{}

void CHPGreenItem::Start()
{
	CItem::Start();
}

bool CHPGreenItem::Init()
{
	if (!CItem::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"GreenItem", TEXT("Item_HP_Green.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("ItemRight");

	SetWorldScale(50.f, 50.f, 1.f);

	return true;
}

void CHPGreenItem::Update(float DeltaTime)
{
	CItem::Update(DeltaTime);
}

void CHPGreenItem::PostUpdate(float DeltaTime)
{
	CItem::PostUpdate(DeltaTime);
}

CHPGreenItem* CHPGreenItem::Clone()
{
	return new CHPGreenItem(*this);
}

void  CHPGreenItem::Save(FILE* pFile)
{
	CItem::Save(pFile);
}