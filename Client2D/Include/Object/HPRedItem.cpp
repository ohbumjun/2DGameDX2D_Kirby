#include "HPRedItem.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Component/SpriteComponent.h"

CHPRedItem::CHPRedItem()
{
	SetTypeID<CHPRedItem>();
	m_ItemType = Item_Type::HPRedPotion;
}

CHPRedItem::CHPRedItem(const CHPRedItem& Monster)
{}

CHPRedItem::~CHPRedItem()
{}

void CHPRedItem::Start()
{
	CItem::Start();
}

bool CHPRedItem::Init()
{
	if (!CItem::Init())
		return false;

	CAnimationSequence2DInstance* AnimationInstance = m_Scene->GetResource()->LoadAnimationInstance(
		"GreenItem", TEXT("Item_HP_Red.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("ItemRight");

	return true;
}

void CHPRedItem::Update(float DeltaTime)
{
	CItem::Update(DeltaTime);
}

void CHPRedItem::PostUpdate(float DeltaTime)
{
	CItem::PostUpdate(DeltaTime);
}

CHPRedItem* CHPRedItem::Clone()
{
	return new CHPRedItem(*this);
}

void  CHPRedItem::Load(FILE* pFile)
{
	CItem::Load(pFile);
}