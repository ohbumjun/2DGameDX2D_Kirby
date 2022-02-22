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
	// Component 세팅
	 m_TileMap = TileMap;

	 m_NodeShape = TileMap->GetTileShape();
	 m_CountX = TileMap->GetTileCountX();
	 m_CountY = TileMap->GetTileCountY();
	 m_TileSize = TileMap->GetTileSize();

	 int Count = m_CountX * m_CountY;

	for (int Index = 0; Index < Count; Index++)
	{
		NavNode* Node = new NavNode;

		Node->TileType = TileMap->GetTile(Index)->GetTileType();
		Node->Pos = TileMap->GetTile(Index)->GetPos();
		Node->Size = m_TileSize;
		Node->Center = Node->Pos + Node->Size * 0.5f;
		Node->IndexX = TileMap->GetTile(Index)->GetIndexX();
		Node->IndexY = TileMap->GetTile(Index)->GetIndexY();
		Node->Index = TileMap->GetTile(Index)->GetIndex();

		m_vecNode.push_back(Node);
	}
 }

 bool CNavigation::FindPath(const Vector3& Start, const Vector3& End, std::list<Vector3>& vecPath)
 {
	 vecPath.clear();

	 int StartIndex = m_TileMap->GetTileIndex(Start);

	 if (StartIndex == -1)
		 return false;

	 int EndIndex = m_TileMap->GetTileIndex(End);

	 if (EndIndex == -1)
		 return false;

	// 이전에 사용하던 노드들이 있을 수도 있으므로
	// 길에 찾기에 사용하던 노드들의 데이터를 초기화 한다.
	 size_t Size = m_vecUseNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecUseNode[i]->NodeType = Nav_Node_Type::None;
		m_vecUseNode[i]->Cost   = FLT_MAX;
		m_vecUseNode[i]->Dist    = FLT_MAX;
		m_vecUseNode[i]->Total   = FLT_MAX;
		m_vecUseNode[i]->Parent = nullptr;
	}

	// 시작 노드와 도착 노드
	NavNode* StartNode = m_vecNode[StartIndex];
	NavNode* EndNode   = m_vecNode[EndIndex];

	// 만약 발견했다면
	if (StartNode == EndNode)
	{
		vecPath.push_back(End);
		return true;
	}

	// 만약 도착 노드가 Wall 이라면 false
	if (EndNode->TileType == Tile_Type::Wall)
		return false;

	// 시작 노드의 정보를 세팅해준다.
	StartNode->NodeType = Nav_Node_Type::Open;
	StartNode->Cost = 0.f;
	StartNode->Dist = StartNode->Pos.Distance(End);
	StartNode->Total = StartNode->Dist + StartNode->Cost;

	// 열린 목록에 추가해준다.
	m_vecOpen.push_back(StartNode);

	// 사용 목록에 추가해준다.
	m_vecUseNode.push_back(StartNode);

	while (!m_vecOpen.empty())
	{
		// 가장 비용이 작은 Open Node를 꺼내온다.
		// 다익스트라 개념 !
		NavNode* CurrentNode = m_vecOpen.back();

		m_vecOpen.pop_back();

		// 8 방향의 타일을 검사하여
		// 코너를 열린 목록에 넣어준다.
		if (FindNode(CurrentNode, EndNode, End, vecPath))
			break;

		// Total 비용을 기준으로 내림차순 정렬한다.
		if (!m_vecOpen.empty())
		{
			std::qsort(&m_vecOpen[0], m_vecOpen.size(), sizeof(NavNode), CNavigation::SortNode);
		}
	}
 }

bool CNavigation::FindNode(NavNode* Node, NavNode* EndNode,  const Vector3& End, std::list<Vector3>& vecPath)
 {
	// 8방향 코너를 탐색한다.
	for (int Dir = 0; Dir < (int)Node_Dir::End; Dir++)
	{
		// 해당 방향으로 코너가 있는지 검사한다.
		NavNode* Corner = GetCorner((Node_Dir)Dir, Node, EndNode, End);

		// 해당 방향으로 코너가 없다면 Skip
		if (!Corner)
			continue;

		// 찾아준 노드가 만약 도착노드라면, 경로를 만들어준다.
		if (Corner == EndNode)
		{
			vecPath.push_front(End);

			// EndNode의 부모를 차례대로 list 앞쪽에 추가해준다.
			NavNode* ParentNode = EndNode->Parent;

			while (ParentNode)
			{
				vecPath.push_front(ParentNode->Center);

				ParentNode = ParentNode->Parent;
			}

			return true; // 도착노드를 찾았음을 표시한다.
		}

		// 해당 방향으로 코너가 있다면

		// 1) Cost 정보등을 Update 하고
		float Cost = 0.f;

		if (m_NodeShape == Tile_Shape::Rect)
		{
			switch ((Node_Dir)Dir)
			{
				case Node_Dir::R :
				case Node_Dir::L :
					Cost = Node->Cost + std::abs(Node->Pos.x - Corner->Pos.x);
					break;
				case Node_Dir::B:
				case Node_Dir::T:
					Cost = Node->Cost + std::abs(Node->Pos.y - Corner->Pos.y);
					break;
				case Node_Dir::RT:
				case Node_Dir::LT:
				case Node_Dir::RB:
				case Node_Dir::LB:
					Cost = Node->Cost + Node->Pos.Distance(Corner->Pos);
					break;
			}
		}
		// 마름모
		else
		{
			switch ((Node_Dir)Dir)
			{
			case Node_Dir::R:
			case Node_Dir::L:
				Cost = Node->Cost + std::abs(Node->Pos.x - Corner->Pos.x);
				break;
			case Node_Dir::B:
			case Node_Dir::T:
				Cost = Node->Cost + std::abs(Node->Pos.y - Corner->Pos.y);
				break;
			case Node_Dir::RT:
			case Node_Dir::LT:
			case Node_Dir::RB:
			case Node_Dir::LB:
				Cost = Node->Cost + Node->Pos.Distance(Corner->Pos);
				break;
			}
		}

		// 2) 열린 목록으로 추가해줄 것이다.

		// 이미 열린 목록에 추가되어 있는 녀석이라면
		// Cost 정보 Update
		if (Corner->NodeType == Nav_Node_Type::Open)
		{
			if (Corner->Cost > Cost)
			{
				Corner->Cost = Cost;
				Corner->Total = Corner->Cost + Corner->Dist;
				Corner->Parent = Node;
			}
		}
		else
		{
			Corner->Cost = Cost;
			Corner->Total = Cost + Corner->Dist;
			Corner->NodeType = Nav_Node_Type::Open;
			Corner->Parent = Node;
		}
	}
 }

NavNode* CNavigation::GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End)
 {
	 switch (m_NodeShape)
	 {
	 case Tile_Shape::Rect :
		 switch (Dir)
		 {
		 case Node_Dir::T:
			 return GetRectNodeTop(Node, EndNode, End,false);
		 case Node_Dir::RT:
			 return GetRectNodeRightTop(Node, EndNode, End);
		 case Node_Dir::R:
			 return GetRectNodeRight(Node, EndNode, End,false);
		 case Node_Dir::RB:
			 return GetRectNodeRightBottom(Node, EndNode, End);
		 case Node_Dir::B:
			 return GetRectNodeBottom(Node, EndNode, End,false);
		 case Node_Dir::LB:
			 return GetRectNodeLeftBottom(Node, EndNode, End);
		 case Node_Dir::L:
			 return GetRectNodeLeft(Node, EndNode, End,false);
		 case Node_Dir::LT:
			 return GetRectNodeLeftTop(Node, EndNode, End);
		 }
		 break;
	 case Tile_Shape::Rhombus :
		 {
		 
		 }
		 break;
	 }
 }


NavNode* CNavigation::GetRectNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End,
	bool Digonal)
{
	// 위로 이동할때는 노드를 한칸씩 위로 이동을 시키면서 해당 노드의 오른쪽이 막혀있고 오른쪽 위가 뚫려있거나
	// 왼쪽이 막혀있고 왼쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
	int	IndexY = Node->IndexY;

	while (true)
	{
		++IndexY;

		if (IndexY + 1 >= m_CountY)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + Node->IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = Node->IndexX + 1;
		int	CornerY = IndexY;

		if (CornerX < m_CountX)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = Node->IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, 
	const Vector3& End, bool Diagonal)
{
	// 오른쪽 위로 이동할때는 노드를 한칸씩 오른쪽 위로 이동을 시키면서 해당 노드의 왼이 막혀있고 왼쪽 위가 뚫려있거나
	// 아래쪽이 막혀있고 오른쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
	int	IndexY = Node->IndexY;
	int	IndexX = Node->IndexX;

	while (true)
	{
		++IndexY;
		++IndexX;

		if (IndexY + 1 >= m_CountY || IndexX + 1 >= m_CountX)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = IndexX - 1;
		int	CornerY = IndexY;

		if (CornerX >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = IndexX;
		CornerY = IndexY - 1;

		if (CornerY >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽 위 대각선 체크시 만약 현 노드가 코너가 아니라면 오른쪽 방향과 위쪽 방향을 체크하여
		// 코너가 있는지를 판단한다.
		NavNode* FindNode = GetRectNodeTop(CheckNode, EndNode, End);

		// 위쪽 검사중 노드를 찾았다면 현재의 노드를 코너로 체크한다.
		if (FindNode)
			return CheckNode;

		FindNode = GetRectNodeRight(CheckNode, EndNode, End);

		if (FindNode)
			return CheckNode;
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal)
{
	// 오른쪽으로 이동할때는 가로가 1씩 증가하며 위가 막혀있고 오른쪽 위는 갈 수 있거나
	// 아래가 막혀있고 오른쪽 아래는 갈 수 있을 경우 코너가 된다.
	int	IndexX = Node->IndexX;

	while (true)
	{
		++IndexX;

		if (IndexX + 1 >= m_CountX)
			return nullptr;

		NavNode* CheckNode = m_vecNode[Node->IndexY * m_CountX + IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = Node->IndexY + 1;

		if (CornerY < m_CountY)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = IndexX;
		CornerY = Node->IndexY - 1;

		if (CornerY >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, 
const Vector3& End, bool Diagonal)
{
	// 오른쪽 아래로 이동할때는 노드를 한칸씩 오른쪽 아래로 이동을 시키면서 해당 노드의 왼쪽이 막혀있고 왼쪽 아래가 뚫려있거나
	// 위쪽이 막혀있고 오른쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
	int	IndexY = Node->IndexY;
	int	IndexX = Node->IndexX;

	while (true)
	{
		--IndexY;
		++IndexX;

		if (IndexY - 1 < 0 || IndexX + 1 >= m_CountX)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = IndexX - 1;
		int	CornerY = IndexY;

		if (CornerX >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = IndexX;
		CornerY = IndexY + 1;

		if (CornerY < m_CountY)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽 아래 대각선 체크시 만약 현 노드가 코너가 아니라면 오른쪽 방향과 아래쪽 방향을 체크하여
		// 코너가 있는지를 판단한다.
		NavNode* FindNode = GetRectNodeBottom(CheckNode, EndNode, End);

		// 위쪽 검사중 노드를 찾았다면 현재의 노드를 코너로 체크한다.
		if (FindNode)
			return CheckNode;

		FindNode = GetRectNodeRight(CheckNode, EndNode, End);

		if (FindNode)
			return CheckNode;
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal)
{
	// 아래로 이동할때는 노드를 한칸씩 아래로 이동을 시키면서 해당 노드의 오른쪽이 막혀있고 오른쪽 아래가 뚫려있거나
	// 왼쪽이 막혀있고 왼쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
	int	IndexY = Node->IndexY;

	while (true)
	{
		--IndexY;

		if (IndexY - 1 < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + Node->IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = Node->IndexX + 1;
		int	CornerY = IndexY;

		if (CornerX < m_CountX)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = Node->IndexX - 1;
		CornerY = IndexY;

		if (CornerX >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, 
const Vector3& End, bool Diagonal)
{
	// 왼쪽 아래로 이동할때는 노드를 한칸씩 왼쪽 아래로 이동을 시키면서 해당 노드의 위쪽이 막혀있고 왼쪽 위가 뚫려있거나
	// 오른쪽이 막혀있고 오른쪽 아래가 뚫려있으면 해당 노드는 코너가 된다.
	int	IndexY = Node->IndexY;
	int	IndexX = Node->IndexX;

	while (true)
	{
		--IndexY;
		--IndexX;

		if (IndexY - 1 < 0 || IndexX - 1 < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY + 1;

		if (CornerY < m_CountY)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = IndexX + 1;
		CornerY = IndexY;

		if (CornerX < m_CountX)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY - 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 왼쪽 아래 대각선 체크시 만약 현 노드가 코너가 아니라면 왼쪽 방향과 아래쪽 방향을 체크하여
		// 코너가 있는지를 판단한다.
		NavNode* FindNode = GetRectNodeBottom(CheckNode, EndNode, End);

		// 위쪽 검사중 노드를 찾았다면 현재의 노드를 코너로 체크한다.
		if (FindNode)
			return CheckNode;

		FindNode = GetRectNodeLeft(CheckNode, EndNode, End);

		if (FindNode)
			return CheckNode;
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End, bool Digonal)
{
	// 왼쪽으로 이동할때는 가로가 1씩 감소하며 위가 막혀있고 왼쪽 위는 갈 수 있거나
	// 아래가 막혀있고 왼쪽 아래는 갈 수 있을 경우 코너가 된다.
	int	IndexX = Node->IndexX;

	while (true)
	{
		--IndexX;

		if (IndexX - 1 < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[Node->IndexY * m_CountX + IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = Node->IndexY + 1;

		if (CornerY < m_CountY)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = IndexX;
		CornerY = Node->IndexY - 1;

		if (CornerY >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}

	return nullptr;
}

NavNode* CNavigation::GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, 
const Vector3& End, bool Diagonal)
{
	// 왼쪽 위로 이동할때는 노드를 한칸씩 왼쪽 위로 이동을 시키면서 해당 노드의 아래쪽이 막혀있고 왼쪽 아래가 뚫려있거나
	// 오른쪽이 막혀있고 오른쪽 위가 뚫려있으면 해당 노드는 코너가 된다.
	int	IndexY = Node->IndexY;
	int	IndexX = Node->IndexX;

	while (true)
	{
		++IndexY;
		--IndexX;

		if (IndexY + 1 >= m_CountY || IndexX - 1 < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		if (CheckNode == EndNode)
			return CheckNode;

		else if (CheckNode->NodeType == Nav_Node_Type::Close)
			return nullptr;

		else if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		int	CornerX = IndexX;
		int	CornerY = IndexY - 1;

		if (CornerY >= 0)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[CornerY * m_CountX + (CornerX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		CornerX = IndexX + 1;
		CornerY = IndexY;

		if (CornerX < m_CountX)
		{
			if (m_vecNode[CornerY * m_CountX + CornerX]->TileType == Tile_Type::Wall &&
				m_vecNode[(CornerY + 1) * m_CountX + CornerX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 왼쪽 위 대각선 체크시 만약 현 노드가 코너가 아니라면 왼쪽 방향과 위쪽 방향을 체크하여
		// 코너가 있는지를 판단한다.
		NavNode* FindNode = GetRectNodeTop(CheckNode, EndNode, End);

		// 위쪽 검사중 노드를 찾았다면 현재의 노드를 코너로 체크한다.
		if (FindNode)
			return CheckNode;

		FindNode = GetRectNodeLeft(CheckNode, EndNode, End);

		if (FindNode)
			return CheckNode;
	}

	return nullptr;
}


int CNavigation::SortNode(const void* Src, const void* Dest)
 {
	NavNode* SrcNode   = (NavNode*)Src;
	NavNode* DestNode = (NavNode*)Dest;

	// 내림 차순 ?

	if (SrcNode->Total > DestNode->Total)
		return -1;
	else if (SrcNode->Total < DestNode->Total)
		return 1;
	return 0;
 }
