#include "HPGreenItem.h"

CHPGreenItem::CHPGreenItem()
{
	SetTypeID<CHPGreenItem>();
	m_ItemType = Item_Type::HPGreenPotion;
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

void CHPGreenItem::Load(FILE* pFile)
{
	CItem::Load(pFile);
}
