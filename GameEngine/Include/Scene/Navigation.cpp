#include "Navigation.h"

 CNavigation::CNavigation()
{}

 CNavigation::~CNavigation()
 {
	// 전체 노드
	 size_t Size = m_vecNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecNode[i]);
	}
 }

 void CNavigation::CreateNavigationNode(CTileMapComponent* TileMap)
 {
	 m_TileMap = TileMap;
	 m_CountX = TileMap->GetTileCountX();
	 m_CountY = TileMap->GetTileCountY();
	 m_TileSize = TileMap->GetTileSize();

	 int Count = m_CountX * m_CountY;

	 m_vecNode.reserve(Count);

	for (int i = 0; i < Count; i++)
	{
		NavNode* Node = new NavNode;

		Node->TileType = TileMap->GetTile(i)->GetTileType();
		Node->Pos     = TileMap->GetTile(i)->GetPos();
		Node->Center = TileMap->GetTile(i)->GetCenter();
		Node->Size = m_TileSize;
		Node->IndexX = TileMap->GetTile(i)->GetIndexX();
		Node->IndexY = TileMap->GetTile(i)->GetIndexY();
		Node->Index = TileMap->GetTile(i)->GetIndex();

		m_vecNode.push_back(Node);
	}
 }

 bool CNavigation::FindPath(const Vector3& Start, const Vector3& End, std::vector<Vector3>& vecPath)
 {
	 vecPath.clear();

	 int StartIndex = m_TileMap->GetTileIndex(Start);

	 if (StartIndex == -1)
		 return false;

	 int EndIndex = m_TileMap->GetTileIndex(End);

	 if (EndIndex == -1)
		 return false;

	// 이전 길찾기 로직을 통해, 사용했던 노드들이 있을 수도 있으므로
	// 길을 찾기 전에 사용하던 노드들의 데이터를 초기화 해준다
	 size_t Size = m_vecUseNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecUseNode[i]->NodeType = Nav_Node_Type::None;
		m_vecUseNode[i]->Cost = FLT_MAX;
		m_vecUseNode[i]->Dist = FLT_MAX;
		m_vecUseNode[i]->Total = FLT_MAX;
		m_vecUseNode[i]->Parent = nullptr;
	}

	// 시작 노드, 목표 노드 구하기
	NavNode* StartNode = m_vecNode[StartIndex];
	NavNode* EndNode = m_vecNode[EndIndex];

	// 목표 노드가 혹시나 벽이라면 false
	if (EndNode->TileType == Tile_Type::Wall)
		return false;

	// 출발 노드, 끝노드가 동일하다면 도착 처리
	else if (StartNode == EndNode)
	{
		vecPath.push_back(End);
		return true;
	}

	// 시작 노드를 열린 목록에 포힘시킨다.
	StartNode->NodeType = Nav_Node_Type::Open;

	m_vecOpen.push_back(StartNode);

	// 시작 노드를 현재 프레임에서 점검한 노드 목록에 추가해준다.
	m_vecUseNode.push_back(StartNode);

	while (!m_vecOpen.empty())
	{
		NavNode* Node = m_vecOpen.front();

		m_vecOpen.pop_back();

		// 해당 목록을 닫힌 목록으로 바꿔준다. --> 방문 처리의 개념
		Node->NodeType = Nav_Node_Type::Close;

		// 해당 노드를 기준으로 주변 8방향 탐색을 한다.

		// 열린 목록을 정렬한다. --> 비용이 작은 노드가 가장 마지막 노드가 되도록 내림차순으로 정렬한다.
		if (!m_vecOpen.empty())
		{
			std::sort(m_vecUseNode.begin(), m_vecUseNode.end(), SortNode);
		}
	}

	 return false;
 }

bool CNavigation::FindNode(NavNode* Node, NavNode* EndNode,  const Vector3& End, std::vector<Vector3>& vecPath)
 {
	// 주변 8방향을 돌면서, 코너를 찾는다.
	 for (int i = 0; i < (int)Node_Dir::End; i++)
	 {
		 NavNode* Corner = GetCorner((Node_Dir)i, Node, EndNode, End, vecPath);

		 if (!Corner)
			 continue;
	 }
 }

NavNode* CNavigation::GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath)
 {
	 switch (m_NodeShape)
	 {
	 case Tile_Shape::Rect :
		 switch (Dir)
		 {
		 case Node_Dir::T:
			 return GetRectNodeTop(Node, EndNode, End, vecPath, false);
		 case Node_Dir::RT:
			 return GetRectNodeRightTop(Node, EndNode, End, vecPath);
		 case Node_Dir::R:
			 return GetRectNodeRight(Node, EndNode, End, vecPath, false);
		 case Node_Dir::RB:
			 return GetRectNodeRightBottom(Node, EndNode, End, vecPath);
		 case Node_Dir::B:
			 return GetRectNodeBottom(Node, EndNode, End, vecPath, false);
		 case Node_Dir::LB:
			 return GetRectNodeLeftBottom(Node, EndNode, End, vecPath);
		 case Node_Dir::L:
			 return GetRectNodeLeft(Node, EndNode, End, vecPath, false);
		 case Node_Dir::LT:
			 return GetRectNodeLeftTop(Node, EndNode, End, vecPath);
		 }
		 break;
	 case Tile_Shape::Rhombus :
		 {
		 
		 }
		 break;
	 }
 }

NavNode* CNavigation::GetRectNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End, std::vector<Vector3>& vecPath,
	bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

NavNode* CNavigation::GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End,
	std::vector<Vector3>& vecPath, bool Diagonal)
{
	return nullptr;
}

bool CNavigation::SortNode(NavNode* Src, NavNode* Dest)
 {
	return Src->Total > Dest->Total;
 }
