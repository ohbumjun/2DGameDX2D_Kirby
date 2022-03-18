#include "Item.h"

CItem::CItem()
{
	m_ItemType = Item_Type::HPYellowPotion;
}

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

	m_Sprite = CreateComponent<CSpriteComponent>("ItemSprite");
	SetRootComponent(m_Sprite);

	// m_ColliderBody = CreateComponent<CColliderBox2D>("ColliderBody");
	m_ColliderBody = CreateComponent<CColliderCircle>("ColliderBody");
	m_ColliderBody->SetCollisionProfile("MonsterAttack");

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

void CItem::Save(FILE* pFile)
{
	CGameObject::Save(pFile);

	fwrite(&m_ItemType, sizeof(Item_Type), 1, pFile);
}