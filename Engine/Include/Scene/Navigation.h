#pragma once

#include "../Component/TileMapComponent.h"

struct NavNode {
	Nav_Node_Type NodeType;
	Tile_Type TileType;
	Vector3 Pos;
	Vector3 Size;
	Vector3 Center;
	int IndexX;
	int IndexY;
	int Index;
	float Cost;
	float Dist;
	float Total;

	NavNode() :
	NodeType(Nav_Node_Type::None),
	TileType(Tile_Type::Normal),
	IndexX(-1),
	IndexY(-1),
	Index(-1),
	Cost(0.f),
	Dist(0.f),
	Total(0.f){}
};

class CNavigation {
	friend class CNavigationThread;

private :
	CNavigation();
	~CNavigation();
private :
	Tile_Shape m_NodeShape;
	std::vector<NavNode*> m_vecNode;
	int m_CountX;
	int m_CountY;
	Vector3 m_TileSize;
	CSharedPtr<CTileMapComponent> m_TileMap;
public :
	void CreateNavigationNode(CTileMapComponent* TileMap);
	bool FindPath(const Vector3& Start, const Vector3& End, std::vector<Vector3>& vecPath);
};


