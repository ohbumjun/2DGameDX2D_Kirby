#include "TileEmpty.h"

CTileEmpty::CTileEmpty() :
	m_TileType(Tile_Type::Normal),
	m_Owner(nullptr),
	m_IndexX(-1),
	m_IndexY(-1),
	m_Index(-1)
{}

CTileEmpty::CTileEmpty(const CTileEmpty& Tile)
{
	*this = Tile;
}

CTileEmpty::~CTileEmpty()
{}

void CTileEmpty::Start()
{
	m_Center = m_Pos + (m_Size) / 2.f;
}

void CTileEmpty::Update(float DeltaTime)
{
	Vector3 Pos = m_Pos + m_Owner->GetWorldPos();

	Matrix matScale, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);

	matTranslate.Translation(Pos);

	m_matWorld = matScale * matTranslate;
}

void CTileEmpty::Save(FILE* File)
{
	fwrite(&m_TileType, sizeof(Tile_Type), 1, File);
	fwrite(&m_Pos, sizeof(Vector3), 1, File);
	fwrite(&m_Size, sizeof(Vector3), 1, File);
	fwrite(&m_Center, sizeof(Vector3), 1, File);

	fwrite(&m_IndexX, sizeof(int), 1, File);
	fwrite(&m_IndexY, sizeof(int), 1, File);
	fwrite(&m_Index, sizeof(int), 1, File);
}

void CTileEmpty::Load(FILE* File)
{
	fread(&m_TileType, sizeof(Tile_Type), 1, File);
	fread(&m_Pos, sizeof(Vector3), 1, File);
	fread(&m_Size, sizeof(Vector3), 1, File);
	fread(&m_Center, sizeof(Vector3), 1, File);

	fread(&m_IndexX, sizeof(int), 1, File);
	fread(&m_IndexY, sizeof(int), 1, File);
	fread(&m_Index, sizeof(int), 1, File);
}
