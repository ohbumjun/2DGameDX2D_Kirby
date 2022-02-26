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
	std::list<Node_Dir> SearchDirList;

	NavNode() :
		Parent(nullptr),
		NodeType(Nav_Node_Type::None),
		TileType(Tile_Type::Normal),
		IndexX(-1),
		IndexY(-1),
		Index(-1),
		Cost(FLT_MAX),
		Dist(FLT_MAX),
		Total(FLT_MAX)
	{
	}
};

class CNavigation {
	friend class CNavigationThread;

private :
	CNavigation();
	~CNavigation();
private :
	Tile_Shape m_NodeShape;
	int m_CountX;
	int m_CountY;
	std::vector<NavNode*> m_vecNode;
	Vector3 m_TileSize;
	CSharedPtr<CTileMapComponent> m_TileMap;
	std::vector<NavNode*> m_vecOpen;
	std::vector<NavNode*> m_vecUseNode;
public :
	void CreateNavigationNode(class CTileMapComponent* TileMap);
	bool FindPath(const Vector3& Start, const Vector3& End, std::list<Vector3>& vecPath);
private :
	bool FindNode(NavNode* Node, NavNode* EndNode, const Vector3& End, std::list<Vector3>& vecPath);
	NavNode* GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End);
private :
	void AddDir(Node_Dir Dir, NavNode* Node);
private :
	NavNode* GetRectNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Diagonal = false);
	NavNode* GetRectNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Diagonal = false);
	NavNode* GetRectNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Diagonal = false);
	NavNode* GetRectNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Diagonal = false);
	NavNode* GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
private :
	NavNode* GetRhombusNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End);
	NavNode* GetRhombusNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End);

private:
	static bool SortNode(NavNode* Src, NavNode* Dest);
};

