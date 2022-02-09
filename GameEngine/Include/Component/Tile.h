#pragma once

#include "../Engine.h"
#include "../Animation/AnimationSequence2DInstance.h"

class CTile
{
	friend class CTileMapComponent;
private :
	CTile();
	CTile(const CTile& Tile);
	~CTile();
private :
	class CTileMapComponent* m_Owner;
	Matrix m_matWorld;
	Tile_Shape m_TileShape;
	Tile_Type m_TileType;
	Vector3 m_Size;
	Vector3 m_Pos;
	Vector3 m_Center;
	int m_IndexX;
	int m_IndexY;
	int m_IndexZ;
	int m_Index;
	CAnimationSequence2DInstance* m_AnimInstance;
public :
	Matrix GetWorldMatrix() const
{
		return m_matWorld;
}
	Vector3 GetPos() const
{
		return m_Pos;
}
	Vector3 GetSize() const
	{
		return m_Size;
	}
	Vector3 GetCenter() const
	{
		return m_Center;
	}
	int GetIndex() const
{
		return m_Index;
}
	int GetIndexX() const
{
		return m_IndexX;
}
	int GetIndexY() const
	{
		return m_IndexY;
	}
	int GetIndexZ() const
	{
		return m_IndexZ;
	}
	Tile_Shape GetTileShape() const
{
	return m_TileShape;
}
	Tile_Type GetTileType() const
{
	return m_TileType;
}
public :
	void SetIndex(int IndexX, int IndexY, int Index)
	{
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
	}

	void SetShape(Tile_Shape Shape)
	{
		m_TileShape = Shape;
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
public :
	void SetFrameData(const Vector2& StartPos, const Vector2& Size, int Index = 0);
	void AddAnimation(const std::string& AnimationName, CAnimationSequence2DData* Animation);
	void AddAnimation(const std::string& AnimationName, const std::string& SequenceName,
		CAnimationSequence2D* Sequence2D, bool Loop = true, float  PlayTime = 1.f,
		float PlayScale = 1.f, bool Reverse = false);
	void AddAnimation(const std::string& SequenceName, const std::string& AnimationName, bool Loop = true,
		float PlayTime = 1.f,
		float              PlayScale = 1.f, bool Reverse = false);
	void AddAnimation(const TCHAR* FileName, const std::string& PathName,
		const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float              PlayScale = 1.f, bool Reverse = false);
	void                      SetPlayTime(const std::string& Name, float PlayTime);
	void                      SetPlayScale(const std::string& Name, float PlayScale);
	void                      SetReverse(const std::string& Name, bool Reverse);
	void                      SetLoop(const std::string& Name, bool Loop);
	void                      SetCurrentAnimation(const std::string& Name);
	void                      ChangeAnimation(const std::string& Name);
	bool                      CheckCurrentAnimation(const std::string& Name);
	CAnimationSequence2DData* FindAnimationSequence2DData(const std::string& Name);
public :
	void Update(float DeltaTime);
};

