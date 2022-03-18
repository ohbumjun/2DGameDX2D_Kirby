#include "Item.h"

CItem::CItem()
{}

CItem::CItem(const CItem& Monster)
{}

CItem::~CItem()
{}

void CItem::Start()
{
	CGameObject::Start();
}

bool CItem::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CItem::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CItem::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}


void CItem::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	fread(&m_ItemType, sizeof(Item_Type), 1, pFile);
}