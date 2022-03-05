#pragma once

#include "../Engine.h"

class CTileEmpty
{
	friend class CTileEmptyComponent;
private :
	CTileEmpty();
	CTileEmpty(const CTileEmpty& Tile);
	~CTileEmpty();
private :
	class CTileEmptyComponent* m_Owner;
	Tile_Type m_TileType;
	Vector3 m_Pos;
	Vector3 m_Center;
	Vector3 m_Size;
	Matrix m_matWorld;
	int m_IndexX;
	int m_IndexY;
	int m_Index;
	bool m_IsSideCollision;
public:
	bool IsSideCollision () const
	{
		return m_IsSideCollision;
	}
	Tile_Type GetTileType()	const
	{
		return m_TileType;
	}
	const Vector3& GetSize()	const
	{
		return m_Size;
	}
	const Vector3& GetCenter()	const
	{
		return m_Center;
	}
	const Vector3& GetPos()	const
	{
		return m_Pos;
	}

	const Vector3& GetWorldPos() const;

	const Matrix&  GetWorldMatrix()	const
	{
		return m_matWorld;
	}
	int GetIndexX()	const
	{
		return m_IndexX;
	}
	int GetIndexY()	const
	{
		return m_IndexY;
	}
	int GetIndex()	const
	{
		return m_Index;
	}

public:
	void SetIndex(int IndexX, int IndexY, int Index)
	{
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
	}
	void SetTileType(Tile_Type Type)
	{
		m_TileType = Type;
	}
	void SetPos(const Vector3& Pos)
	{
		m_Pos = Pos;
	}
	void SetPos(float x, float y, float z)
	{
		m_Pos = Vector3(x, y, z);
	}
	void SetSize(const Vector3& Size)
	{
		m_Size = Size;
	}
	void SetSize(float x, float y, float z)
	{
		m_Size = Vector3(x, y, z);
	}
	void SetOwner(class CTileEmptyComponent* Owner)
	{
		m_Owner = Owner;
	}
public :
	void Start();
	void Update(float DeltaTime);
	void Save(FILE* File);
	void Load(FILE* File);
};

