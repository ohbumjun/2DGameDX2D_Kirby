#include "HPYellowItem.h"

CHPYellowItem::CHPYellowItem()
{
	SetTypeID<CHPYellowItem>();
	m_ItemType = Item_Type::HPYellowPotion;
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

void  CHPYellowItem::Load(FILE* pFile)
{
	CItem::Load(pFile);
}