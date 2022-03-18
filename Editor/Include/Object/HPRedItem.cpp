#include "HPRedItem.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CHPRedItem::CHPRedItem()
{
	m_ItemType = Item_Type::HPRedPotion;
	SetTypeID<CHPRedItem>();
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
		"RedItem", TEXT("Item_HP_Red.anim"));

	// Clone 해서 세팅해줘야 한다.
	m_Sprite->SetAnimationInstance(AnimationInstance);
	m_Sprite->GetAnimationInstance()->SetCurrentAnimation("ItemRight");

	SetWorldScale(50.f, 50.f, 1.f);

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

void  CHPRedItem::Save(FILE* pFile)
{
	CItem::Save(pFile);
}