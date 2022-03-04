#include "TileMap.h"

#include <Scene/Scene.h>

CTileMap::CTileMap()
{
	SetTypeID<CTileMap>();
}

CTileMap::CTileMap(const CTileMap& Map)
{
	m_TileComponent = (CTileMapComponent*)FindComponent("TileMap");
}

CTileMap::~CTileMap()
{}

void CTileMap::Start()
{
	CGameObject::Start();
}

bool CTileMap::Init()
{
	if (!CGameObject::Init())
		return false;

	// Tile Component
	m_TileComponent = CreateComponent<CTileMapComponent>("TileMap");

	SetRootComponent(m_TileComponent);

	// Back Material
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("BackMaterial");
	m_TileComponent->SetBackMaterial(Material);

	// Tile
	m_TileComponent->CreateTile(Tile_Shape::Rhombus, 1, 1, Vector3(160.f, 80.f, 0.f));

	Material = m_Scene->GetResource()->FindMaterial("DiabloTileMap");

	m_TileComponent->SetTileMaterial(Material);

	m_TileComponent->SetTileDefaultFrame(160.f, 80.f, 320.f, 160.f);
	// Back Material

	return true;
}

void CTileMap::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTileMap::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CTileMap::PrevRender()
{
	CGameObject::PrevRender();
}

void CTileMap::Render()
{
	CGameObject::Render();
}

void CTileMap::PostRender()
{
	CGameObject::PostRender();
}

CTileMap* CTileMap::Clone()
{
	return new CTileMap(*this);
}

void CTileMap::Save(FILE* pFile)
{
	CGameObject::Save(pFile);
}

void CTileMap::Load(FILE* pFile)
{
	CGameObject::Load(pFile);
}
