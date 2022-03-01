#include "TileMapEmpty.h"
#include "Scene/Scene.h"

CTileMapEmpty::CTileMapEmpty()
{}

CTileMapEmpty::CTileMapEmpty(const CTileMapEmpty& Map)
{
	m_TileEmptyComponent = (CTileEmptyComponent*)FindComponent("TileEmpty");
}

CTileMapEmpty::~CTileMapEmpty()
{}

void CTileMapEmpty::Start()
{
	CGameObject::Start();
}

bool CTileMapEmpty::Init()
{
	if (!CGameObject::Init())
		return false;

	m_TileEmptyComponent = CreateComponent<CTileEmptyComponent>("TileEmpty");

	// Tile
	m_TileEmptyComponent->CreateTileEmpty(10, 10, Vector3(50.f, 50.f, 0.f));


	return true;
}

void CTileMapEmpty::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTileMapEmpty::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CTileMapEmpty::PrevRender()
{
	CGameObject::PrevRender();
}

void CTileMapEmpty::Render()
{
	CGameObject::Render();
}

void CTileMapEmpty::PostRender()
{
	CGameObject::PostRender();
}

CTileMapEmpty* CTileMapEmpty::Clone()
{
	return new CTileMapEmpty(*this);
}

void CTileMapEmpty::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CTileMapEmpty::Load(FILE* pFile)
{
	CGameObject::Load(pFile);

	m_TileEmptyComponent = (CTileEmptyComponent*)m_RootComponent.Get();
}
