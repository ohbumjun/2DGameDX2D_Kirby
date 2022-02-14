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
	Tile_Shape m_Shape;
	Tile_Type m_TileType;
	Vector3 m_Pos;
	Vector3 m_Size;
	Vector3 m_Center;
	Matrix m_matWorld;
	CAnimationSequence2DInstance* m_AnimInstance;
	int m_IndexX;
	int m_IndexY;
	int m_IndexZ;
	int m_Index;
	Vector2 m_FrameStart;
	Vector2 m_FrameEnd;
	float m_Opacity;

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
	return m_Shape;
}
	Tile_Type GetTileType() const
{
	return m_TileType;
}
	Vector2 GetFrameStart() const
{
		return m_FrameStart;
}
	Vector2 GetFrameEnd() const
{
		return m_FrameEnd;
}
	bool GetRender() const
{
	// 하나라도 0이 아니라면 그릴 것이 있다는 것
	// 처음에 아예 m_FrameStart, End가 모두 0으로 세팅되어 있기 때문이다.
		return m_FrameStart.x != 0.f || m_FrameStart.y != 0.f || m_FrameEnd.x != 0.f || m_FrameEnd.y != 0.f;
}
	float GetOpacity() const
{
		return m_Opacity;
}
public :
	void SetOpacity(float Opacity)
{
		m_Opacity = Opacity;
}
	void SetIndex(int IndexX, int IndexY, int Index)
	{
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
	}

	void SetShape(Tile_Shape Shape)
	{
		m_Shape = Shape;
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
	void SetFrameStart(float x, float y);
	void SetFrameEnd(float x, float y);
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
	void Start();
	void Update(float DeltaTime);
};

