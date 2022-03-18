#include "HPRedItem.h"

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