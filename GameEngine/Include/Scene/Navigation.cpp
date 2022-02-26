#include "Navigation.h"

CNavigation::CNavigation()
{}

CNavigation::~CNavigation()
{
	size_t Size = m_vecNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecNode[i]);
	}
}

void CNavigation::CreateNavigationNode(CTileMapComponent* TileMap)
{
	m_NodeShape = TileMap->GetTileShape();

	m_CountX = TileMap->GetTileCountX();
	m_CountY = TileMap->GetTileCountY();

	m_TileSize = TileMap->GetTileSize();

	int Count = m_CountX * m_CountY;

	for (int Index = 0; Index < Count; Index++)
	{
		NavNode* Node = new NavNode;

		CTile* Tile = TileMap->GetTile(Index);

		Node->Center = Tile->GetCenter();
		Node->Pos = Tile->GetPos();
		Node->Size = m_TileSize;
		Node->IndexX = Tile->GetIndexX();
		Node->IndexY = Tile->GetIndexY();
		Node->Index = Tile->GetIndex();
		Node->TileType = Tile->GetTileType();

		m_vecNode.push_back(Node);
	}
}

bool CNavigation::FindPath(const Vector3& Start, const Vector3& End, std::list<Vector3>& vecPath)
{
	if (!m_TileMap)
		return false;

	int StartIndex = m_TileMap->GetTileIndex(Start);

	if (StartIndex == -1)
		return false;

	int EndIndex = m_TileMap->GetTileIndex(End);

	if (EndIndex == -1)
		return false;

	// 이전에 길찾기에 사용했던 Node 목록 정보를 초기화 해준다.
	size_t Size = m_vecUseNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecUseNode[i]->NodeType == Nav_Node_Type::None;
		m_vecUseNode[i]->Cost = FLT_MAX;
		m_vecUseNode[i]->Dist = FLT_MAX;
		m_vecUseNode[i]->Total = FLT_MAX;
		m_vecUseNode[i]->Parent = nullptr;
		m_vecUseNode[i]->SearchDirList.clear();
	}

	NavNode* StartNode = m_vecNode[StartIndex];
	NavNode* EndNode   = m_vecNode[EndIndex];

	if (EndNode->TileType == Tile_Type::Wall)
		return false;

	else if (EndNode == StartNode)
	{
		vecPath.push_back(End);
		return true;
	}

	// StartNode 정보 세팅
	StartNode->NodeType = Nav_Node_Type::Open;
	StartNode->Cost = 0.f;
	StartNode->Dist = StartNode->Pos.Distance(End);
	StartNode->Total = StartNode->Cost + StartNode->Dist;

	for (int i = 0; i < (int)Node_Dir::End; i++)
	{
		StartNode->SearchDirList.push_back((Node_Dir)i);
	}

	m_vecUseNode.push_back(StartNode);

	m_vecOpen.push_back(StartNode);

	while (!m_vecOpen.empty())
	{
		NavNode* Node = m_vecOpen.back();
		m_vecOpen.pop_back();

		// 도착노드를 찾았다면 
		if (FindNode(Node, EndNode, End, vecPath))
			break;

		// 정렬
		if (!m_vecOpen.empty())
			std::sort(m_vecOpen.begin(), m_vecOpen.end(), SortNode);
	}

	m_vecOpen.clear();

	return !vecPath.empty();
}

bool CNavigation::FindNode(NavNode* Node, NavNode* EndNode, const Vector3& End, std::list<Vector3>& vecPath)
{
	// 해당 방향을 탐색한다.
	auto iter = Node->SearchDirList.begin();
	auto iterEnd = Node->SearchDirList.end();

	for (; iter != iterEnd; ++iter)
	{
		NavNode* Corner = GetCorner((*iter), Node, EndNode, End);

		if (!Corner)
			continue;

		// 도착 노드라면
		if (Corner == EndNode)
		{
			// 경로를 만들어준다.
			vecPath.push_front(Corner->Center);

			NavNode* PathNode = Node;

			while (PathNode)
			{
				vecPath.push_front(PathNode->Center);
				PathNode = PathNode->Parent;
			}
			return true;
		}

		// 비용 정보 Update
		float Cost = 0.f;

		switch ((*iter))
		{
		case Node_Dir::T:
		case Node_Dir::B:
			Cost = Node->Cost + std::abs(Node->Center.y - Corner->Center.y);
			break;
		case Node_Dir::R:
		case Node_Dir::L:
			Cost = Node->Cost + std::abs(Node->Center.x - Corner->Center.x);
			break;
		case Node_Dir::LT:
		case Node_Dir::LB:
		case Node_Dir::RT:
		case Node_Dir::RB:
			Cost = Node->Cost + Node->Center.Distance(Corner->Center);
			break;
		}

		// 해당 코너가 이미 열린 목록에 들어있는지 여부를 판단하기
		if (Corner->NodeType == Nav_Node_Type::Open)
		{
			if (Corner->Cost > Cost)
			{
				Corner->Cost = Cost;
				Corner->Total = Cost + Corner->Dist;
				Corner->Parent = Node;

				AddDir((*iter), Corner);
			}
		}
		else
		{
			Corner->NodeType = Nav_Node_Type::Open;
			Corner->Cost = Cost;
			Corner->Total = Cost + Corner->Dist;
			Corner->Parent = Node;

			m_vecOpen.push_back(Corner);

			m_vecUseNode.push_back(Corner);

			AddDir((*iter), Corner);
		}
	}

	return false;
}

NavNode* CNavigation::GetCorner(Node_Dir Dir, NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	switch (m_NodeShape)
	{
	case Tile_Shape::Rect:
		switch (Dir)
		{
		case Node_Dir::T:
			return GetRectNodeTop(Node, EndNode, End, false);
		case Node_Dir::RT:
			return GetRectNodeRightTop(Node, EndNode, End);
		case Node_Dir::R:
			return GetRectNodeRight(Node, EndNode, End, false);
		case Node_Dir::RB:
			return GetRectNodeRightBottom(Node, EndNode, End);
		case Node_Dir::B:
			return GetRectNodeBottom(Node, EndNode, End, false);
		case Node_Dir::LB:
			return GetRectNodeLeftBottom(Node, EndNode, End);
		case Node_Dir::L:
			return GetRectNodeLeft(Node, EndNode, End, false);
		case Node_Dir::LT:
			return GetRectNodeLeftTop(Node, EndNode, End);
		}
		break;
	case Tile_Shape::Rhombus:
		switch (Dir)
		{
		case Node_Dir::T:
			return GetRhombusNodeTop(Node, EndNode, End);
		case Node_Dir::RT:
			return GetRhombusNodeRightTop(Node, EndNode, End);
		case Node_Dir::R:
			return GetRhombusNodeRight(Node, EndNode, End);
		case Node_Dir::RB:
			return GetRhombusNodeRightBottom(Node, EndNode, End);
		case Node_Dir::B:
			return GetRhombusNodeBottom(Node, EndNode, End);
		case Node_Dir::LB:
			return GetRhombusNodeLeftBottom(Node, EndNode, End);
		case Node_Dir::L:
			return GetRhombusNodeLeft(Node, EndNode, End);
		case Node_Dir::LT:
			return GetRhombusNodeLeftTop(Node, EndNode, End);
		}
		break;
	}

	return nullptr;
}

void CNavigation::AddDir(Node_Dir Dir, NavNode* Node)
{
	Node->SearchDirList.clear();

	switch (Dir)
	{
		case Node_Dir::L :
			Node->SearchDirList.push_back(Node_Dir::L);
			Node->SearchDirList.push_back(Node_Dir::LB);
			Node->SearchDirList.push_back(Node_Dir::LT);
			break;
		case Node_Dir::R:
			Node->SearchDirList.push_back(Node_Dir::R);
			Node->SearchDirList.push_back(Node_Dir::RB);
			Node->SearchDirList.push_back(Node_Dir::RT);
			break;
		case Node_Dir::T:
			Node->SearchDirList.push_back(Node_Dir::T);
			Node->SearchDirList.push_back(Node_Dir::RT);
			Node->SearchDirList.push_back(Node_Dir::LT);
			break;
		case Node_Dir::B:
			Node->SearchDirList.push_back(Node_Dir::B);
			Node->SearchDirList.push_back(Node_Dir::LB);
			Node->SearchDirList.push_back(Node_Dir::RB);
			break;
		case Node_Dir::LT:
			Node->SearchDirList.push_back(Node_Dir::L);
			Node->SearchDirList.push_back(Node_Dir::LB);
			Node->SearchDirList.push_back(Node_Dir::LT);
			Node->SearchDirList.push_back(Node_Dir::T);
			Node->SearchDirList.push_back(Node_Dir::RT);
			break;
		case Node_Dir::RT:
			Node->SearchDirList.push_back(Node_Dir::RT);
			Node->SearchDirList.push_back(Node_Dir::T);
			Node->SearchDirList.push_back(Node_Dir::LT);
			Node->SearchDirList.push_back(Node_Dir::R);
			Node->SearchDirList.push_back(Node_Dir::RB);
			break;
		case Node_Dir::LB:
			Node->SearchDirList.push_back(Node_Dir::LT);
			Node->SearchDirList.push_back(Node_Dir::L);
			Node->SearchDirList.push_back(Node_Dir::LB);
			Node->SearchDirList.push_back(Node_Dir::B);
			Node->SearchDirList.push_back(Node_Dir::RB);
			break;
		case Node_Dir::RB:
			Node->SearchDirList.push_back(Node_Dir::RT);
			Node->SearchDirList.push_back(Node_Dir::R);
			Node->SearchDirList.push_back(Node_Dir::RB);
			Node->SearchDirList.push_back(Node_Dir::B);
			Node->SearchDirList.push_back(Node_Dir::LB);
			break;
	}
}

NavNode* CNavigation::GetRectNodeTop(NavNode* Node, NavNode* EndNode, 
	const Vector3& End, bool Diagonal)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexY += 1;

		// 범위를 벗어났다면 
		if (IndexY >= m_CountY)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽은 벽, 왼쪽 위는 벽 X
		int ConvertX = IndexX - 1;
		int ConvertY = IndexY;

		if (ConvertX >= 0 && ConvertY + 1 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY + 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽은 벽, 오른쪽 위는 벽 X
		ConvertX = IndexX + 1;
		ConvertY = IndexY;

		if (ConvertX >= 0 && ConvertY + 1 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY + 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRectNodeRight(NavNode* Node, NavNode* EndNode, 
	const Vector3& End, bool Diagonal)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexX += 1;

		// 범위를 벗어났다면 
		if (IndexX >= m_CountX)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 위는 벽, 오른쪽 위는 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY + 1;

		if (ConvertX + 1 < m_CountX && ConvertY < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 아래는 벽, 오른쪽 아래는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY - 1;

		if (ConvertX + 1 < m_CountX && ConvertY >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRectNodeLeft(NavNode* Node, NavNode* EndNode, 
	const Vector3& End, bool Diagonal)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexX -= 1;

		// 범위를 벗어났다면 
		if (IndexX < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 위는 벽, 왼쪽 위는 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY + 1;

		if (ConvertX - 1 >= 0 && ConvertY < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 아래는 벽, 왼쪽 아래는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY - 1;

		if (ConvertX - 1 >= 0 && ConvertY >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRectNodeBottom(NavNode* Node, NavNode* EndNode, 
	const Vector3& End, bool Diagonal)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexY -= 1;

		// 범위를 벗어났다면 
		if (IndexY < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽은 벽, 왼쪽 아래는 벽 X
		int ConvertX = IndexX - 1;
		int ConvertY = IndexY;

		if (ConvertX >= 0 && ConvertY - 1 >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY - 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽은 벽, 오른쪽 아래는 벽 X
		ConvertX = IndexX + 1;
		ConvertY = IndexY;

		if (ConvertX < m_CountX && ConvertY - 1 >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY - 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRectNodeRightTop(NavNode* Node, NavNode* EndNode, 
	const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexX += 1;
		IndexY += 1;

		// 범위를 벗어났다면 
		if (IndexX >= m_CountX || IndexY >= m_CountY)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽은 벽, 왼쪽 위는 벽 X
		int ConvertX = IndexX - 1;
		int ConvertY = IndexY;

		if (ConvertX >= 0 && ConvertY + 1 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY + 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 아래는 벽, 오른쪽 아래는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY - 1;

		if (ConvertX + 1 < m_CountX && ConvertY >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽
		NavNode* FindNode = GetRectNodeRight(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;

		// 위쪽
		FindNode = GetRectNodeTop(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;
	}
}

NavNode* CNavigation::GetRectNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexX += 1;
		IndexY -= 1;

		// 범위를 벗어났다면 
		if (IndexX >= m_CountX || IndexY < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽은 벽, 왼쪽 아래는 벽 X
		int ConvertX = IndexX - 1;
		int ConvertY = IndexY;

		if (ConvertX >= 0 && ConvertY - 1 >= 0) 
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY - 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 위는 벽, 오른쪽 위는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY - 1;

		if (ConvertX + 1 < m_CountX && ConvertY < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽
		NavNode* FindNode = GetRectNodeRight(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;

		// 아래쪽
		FindNode = GetRectNodeBottom(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;
	}
}

NavNode* CNavigation::GetRectNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexX -= 1;
		IndexY -= 1;

		// 범위를 벗어났다면 
		if (IndexX >= m_CountX || IndexY < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 위쪽은 벽, 왼쪽 위는 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY + 1;

		if (ConvertX - 1 >= 0 && ConvertY < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽은 벽, 오른쪽 아래는 벽 X
		ConvertX = IndexX + 1;
		ConvertY = IndexY;

		if (ConvertX < m_CountX && ConvertY - 1 >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY - 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 왼쪽
		NavNode* FindNode = GetRectNodeLeft(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;

		// 아래쪽
		FindNode = GetRectNodeBottom(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;
	}
}


NavNode* CNavigation::GetRectNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		IndexX -= 1;
		IndexY += 1;

		// 범위를 벗어났다면 
		if (IndexX < 0 || IndexY >= m_CountY)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착 노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 오른쪽은 벽, 오른쪽 위는 벽 X
		int ConvertX = IndexX + 1;
		int ConvertY = IndexY;

		if (ConvertX < m_CountX && ConvertY + 1 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(ConvertY + 1) * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 아래는 벽, 왼쪽 아래는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY - 1;

		if (ConvertX - 1 >= 0 && ConvertY >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + (ConvertX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 왼쪽
		NavNode* FindNode = GetRectNodeLeft(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;

		// 위쪽 
		FindNode = GetRectNodeTop(CheckNode, EndNode, End, true);

		if (FindNode)
			return CheckNode;
	}
}

NavNode* CNavigation::GetRhombusNodeTop(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
}

NavNode* CNavigation::GetRhombusNodeRightTop(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		// 오른쪽 위로 올라갈 것이다.
		// Y는 무조건 + 1
		// Y가 홀수 일 때는 X + 1을 해준다.
		if (IndexY & 1)
			IndexX += 1;

		IndexY -= 1;

		// 범위를 벗어났다면
		if (IndexX < 0 || IndexY >= m_CountY)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽 위는 벽, 위는 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY + 1;

		// Y가 짝수라면, 왼쪽 아래로 갈 때 X 는 - 1을 해줘야 한다.
		if ((ConvertY & 1) == false)
			ConvertX -= 1;

		if (ConvertX >= 0 && IndexY + 2 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(IndexY + 2) * m_CountY + IndexX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽 아래는 벽, 오른쪽은 벽
		ConvertX = IndexX;
		ConvertY = IndexY - 1;

		// Y가 홀수라면, 오른쪽 아래로 갈 때 X는 + 1
		if (ConvertY & 1)
			ConvertX += 1;

		if (IndexX + 2 < m_CountX && ConvertY >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[IndexY * m_CountY + (IndexX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRhombusNodeRight(NavNode* Node, NavNode* EndNode, const Vector3& End)
{}

NavNode* CNavigation::GetRhombusNodeRightBottom(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		// 오른쪽 아래로 내려갈 것이다.
		// Y는 무조건 - 1
		// Y가 홀수 일 때는 X + 1을 해준다.
		if (IndexY & 1)
			IndexX += 1;

		IndexY -= 1;

		// 범위를 벗어났다면
		if (IndexX >= m_CountX || IndexY < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽 아래는 벽, 아래는 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY - 1;

		// Y가 짝수라면, 왼쪽 아래로 갈 때 X는 - 1을 해줘야 한다.
		if ((ConvertY & 1) == false)
			ConvertX -= 1;

		if (ConvertX >= 0 && ConvertY - 1 >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(IndexY - 2) * m_CountY + IndexX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽 위는 벽, 오른쪽은 벽
		ConvertX = IndexX;
		ConvertY = IndexY + 1;

		// Y가 홀수라면, 오른쪽 위로 갈 때 X는 + 1
		if (ConvertY & 1)
			ConvertX += 1;

		if (ConvertX < m_CountX && IndexY + 2 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[IndexY * m_CountY + (IndexX + 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRhombusNodeBottom(NavNode* Node, NavNode* EndNode, const Vector3& End)
{}

NavNode* CNavigation::GetRhombusNodeLeftBottom(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		// 왼쪽 아래로 내려갈 것이다.
		// Y는 무조건 - 1
		// X 의 경우, Y가 홀수 일때는 그대로
		// Y가 짝수 일 때는 - 1을 해준다.
		if ((IndexY & 1) == false)
			IndexX -= 1;

		IndexY -= 1;

		// 범위를 벗어났다면
		if (IndexX < 0 || IndexY < 0)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽 위는 벽, 왼쪽은 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY;

		// Y가 짝수라면, 왼쪽 위로 갈 때 X는 - 1을 해줘야 한다.
		if ((ConvertY & 1) == false)
			ConvertX -= 1;

		if (ConvertX >= 0 && ConvertY + 1 < m_CountY)
		{
			if (m_vecNode[(ConvertY + 1) * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽 아래는 벽, 아래는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY - 2;

		// Y가 홀수라면, 오른쪽 아래로 갈 때 X는 + 1
		if (ConvertY & 1)
			ConvertX += 1;

		if (ConvertX >= 0 && ConvertY + 1 < m_CountY)
		{
			if (m_vecNode[(IndexY - 1) * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[ConvertY * m_CountY + IndexX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

NavNode* CNavigation::GetRhombusNodeLeft(NavNode* Node, NavNode* EndNode, const Vector3& End) //
{}

NavNode* CNavigation::GetRhombusNodeLeftTop(NavNode* Node, NavNode* EndNode, const Vector3& End)
{
	int IndexX = Node->IndexX;
	int IndexY = Node->IndexY;

	while (true)
	{
		// 왼쪽 위로 올라갈 것이다.
		// Y는 무조건 + 1
		// Y가 짝수 일 때는 X - 1을 해준다.
		if ((IndexY & 1) == false)
			IndexX -= 1;

		IndexY -= 1;

		// 범위를 벗어났다면
		if (IndexX < 0 || IndexY >= m_CountY)
			return nullptr;

		NavNode* CheckNode = m_vecNode[IndexY * m_CountX + IndexX];

		// 벽이라면
		if (CheckNode->TileType == Tile_Type::Wall)
			return nullptr;

		// 도착노드를 찾았다면
		if (CheckNode == EndNode)
			return CheckNode;

		// 왼쪽 아래는 벽, 왼쪽은 벽 X
		int ConvertX = IndexX;
		int ConvertY = IndexY - 1;

		// Y가 짝수라면, 왼쪽 아래로 갈 때 X는 - 1을 해줘야 한다.
		if ((ConvertY & 1) == false)
			ConvertX -= 1;

		if (ConvertX >= 0 && ConvertY - 1 >= 0)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[IndexY * m_CountY + (ConvertX - 1)]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}

		// 오른쪽 위는 벽, 아래는 벽 X
		ConvertX = IndexX;
		ConvertY = IndexY + 1;

		// Y가 홀수라면, 오른쪽 위로 갈 때 X는 + 1
		if (ConvertY & 1)
			ConvertX += 1;

		if (ConvertX < m_CountX && IndexY + 2 < m_CountY)
		{
			if (m_vecNode[ConvertY * m_CountY + ConvertX]->TileType == Tile_Type::Wall &&
				m_vecNode[(IndexY + 2) * m_CountY + IndexX]->TileType == Tile_Type::Normal)
			{
				return CheckNode;
			}
		}
	}
}

bool CNavigation::SortNode(NavNode* Src, NavNode* Dest)
{
	return Src->Total > Dest->Total;
}
