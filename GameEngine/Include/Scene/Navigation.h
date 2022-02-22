#pragma once

#include "../Component/TileMapComponent.h"

enum class Node_Dir {
	T,
	RT,
	R,
	RB,
	B,
	LB,
	L,
	LT,
	End
};

struct NavNode {
	NavNode* Parent;
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
	Parent(nullptr),
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
	std::vector<NavNode*> m_vecOpen;
	std::vector<NavNode*> m_vecUseNode;
public :
	void CreateNavigationNode(CTileMapComponent* TileMap);
	bool FindPath(const Vector3& Start, const Vector3& End, std::list<Vector3>& vecPath);
	bool FindNode(NavNode* Node, NavNode* EndNode, const Vector3& End, std::list<Vector3>& vecPath);
	NavNode* GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End);
private :
	NavNode* GetRectNodeTop(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeRight(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeBottom(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeLeft(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	NavNode* GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, 
		const Vector3& End,bool Diagonal = true);
	
private :
	static int SortNode(const void * Src, const void * Dest);
};


