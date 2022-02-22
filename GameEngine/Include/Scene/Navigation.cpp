#include "Navigation.h"

 CNavigation::CNavigation()
{}

 CNavigation::~CNavigation()
 {
	// ��ü ���
	 size_t Size = m_vecNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		SAFE_DELETE(m_vecNode[i]);
	}
 }

 void CNavigation::CreateNavigationNode(CTileMapComponent* TileMap)
 {
	// Component ����
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

	// ������ ����ϴ� ������ ���� ���� �����Ƿ�
	// �濡 ã�⿡ ����ϴ� ������ �����͸� �ʱ�ȭ �Ѵ�.
	 size_t Size = m_vecUseNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecUseNode[i]->NodeType = Nav_Node_Type::None;
		m_vecUseNode[i]->Cost   = FLT_MAX;
		m_vecUseNode[i]->Dist    = FLT_MAX;
		m_vecUseNode[i]->Total   = FLT_MAX;
		m_vecUseNode[i]->Parent = nullptr;
	}

	// ���� ���� ���� ���
	NavNode* StartNode = m_vecNode[StartIndex];
	NavNode* EndNode   = m_vecNode[EndIndex];

	// ���� �߰��ߴٸ�
	if (StartNode == EndNode)
	{
		vecPath.push_back(End);
		return true;
	}

	// ���� ���� ��尡 Wall �̶�� false
	if (EndNode->TileType == Tile_Type::Wall)
		return false;

	// ���� ����� ������ �������ش�.
	StartNode->NodeType = Nav_Node_Type::Open;
	StartNode->Cost = 0.f;
	StartNode->Dist = StartNode->Pos.Distance(End);
	StartNode->Total = StartNode->Dist + StartNode->Cost;

	// ���� ��Ͽ� �߰����ش�.
	m_vecOpen.push_back(StartNode);

	// ��� ��Ͽ� �߰����ش�.
	m_vecUseNode.push_back(StartNode);

	while (!m_vecOpen.empty())
	{
		// ���� ����� ���� Open Node�� �����´�.
		// ���ͽ�Ʈ�� ���� !
		NavNode* CurrentNode = m_vecOpen.back();

		m_vecOpen.pop_back();

		// 8 ������ Ÿ���� �˻��Ͽ�
		// �ڳʸ� ���� ��Ͽ� �־��ش�.
		if (FindNode(CurrentNode, EndNode, End, vecPath))
			break;

		// Total ����� �������� �������� �����Ѵ�.
		if (!m_vecOpen.empty())
		{
			std::qsort(&m_vecOpen[0], m_vecOpen.size(), sizeof(NavNode), CNavigation::SortNode);
		}
	}
 }

bool CNavigation::FindNode(NavNode* Node, NavNode* EndNode,  const Vector3& End, std::list<Vector3>& vecPath)
 {
	// 8���� �ڳʸ� Ž���Ѵ�.
	for (int Dir = 0; Dir < (int)Node_Dir::End; Dir++)
	{
		// �ش� �������� �ڳʰ� �ִ��� �˻��Ѵ�.
		NavNode* Corner = GetCorner((Node_Dir)Dir, Node, EndNode, End);

		// �ش� �������� �ڳʰ� ���ٸ� Skip
		if (!Corner)
			continue;

		// ã���� ��尡 ���� ���������, ��θ� ������ش�.
		if (Corner == EndNode)
		{
			vecPath.push_front(End);

			// EndNode�� �θ� ���ʴ�� list ���ʿ� �߰����ش�.
			NavNode* ParentNode = EndNode->Parent;

			while (ParentNode)
			{
				vecPath.push_front(ParentNode->Center);

				ParentNode = ParentNode->Parent;
			}

			return true; // ������带 ã������ ǥ���Ѵ�.
		}

		// �ش� �������� �ڳʰ� �ִٸ�

		// 1) Cost �������� Update �ϰ�
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
		// ������
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

		// 2) ���� ������� �߰����� ���̴�.

		// �̹� ���� ��Ͽ� �߰��Ǿ� �ִ� �༮�̶��
		// Cost ���� Update
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
	// ���� �̵��Ҷ��� ��带 ��ĭ�� ���� �̵��� ��Ű�鼭 �ش� ����� �������� �����ְ� ������ ���� �շ��ְų�
	// ������ �����ְ� ���� ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
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
	// ������ ���� �̵��Ҷ��� ��带 ��ĭ�� ������ ���� �̵��� ��Ű�鼭 �ش� ����� ���� �����ְ� ���� ���� �շ��ְų�
	// �Ʒ����� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
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

		// ������ �� �밢�� üũ�� ���� �� ��尡 �ڳʰ� �ƴ϶�� ������ ����� ���� ������ üũ�Ͽ�
		// �ڳʰ� �ִ����� �Ǵ��Ѵ�.
		NavNode* FindNode = GetRectNodeTop(CheckNode, EndNode, End);

		// ���� �˻��� ��带 ã�Ҵٸ� ������ ��带 �ڳʷ� üũ�Ѵ�.
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
	// ���������� �̵��Ҷ��� ���ΰ� 1�� �����ϸ� ���� �����ְ� ������ ���� �� �� �ְų�
	// �Ʒ��� �����ְ� ������ �Ʒ��� �� �� ���� ��� �ڳʰ� �ȴ�.
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
	// ������ �Ʒ��� �̵��Ҷ��� ��带 ��ĭ�� ������ �Ʒ��� �̵��� ��Ű�鼭 �ش� ����� ������ �����ְ� ���� �Ʒ��� �շ��ְų�
	// ������ �����ְ� ������ ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
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

		// ������ �Ʒ� �밢�� üũ�� ���� �� ��尡 �ڳʰ� �ƴ϶�� ������ ����� �Ʒ��� ������ üũ�Ͽ�
		// �ڳʰ� �ִ����� �Ǵ��Ѵ�.
		NavNode* FindNode = GetRectNodeBottom(CheckNode, EndNode, End);

		// ���� �˻��� ��带 ã�Ҵٸ� ������ ��带 �ڳʷ� üũ�Ѵ�.
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
	// �Ʒ��� �̵��Ҷ��� ��带 ��ĭ�� �Ʒ��� �̵��� ��Ű�鼭 �ش� ����� �������� �����ְ� ������ �Ʒ��� �շ��ְų�
	// ������ �����ְ� ���� �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
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
	// ���� �Ʒ��� �̵��Ҷ��� ��带 ��ĭ�� ���� �Ʒ��� �̵��� ��Ű�鼭 �ش� ����� ������ �����ְ� ���� ���� �շ��ְų�
	// �������� �����ְ� ������ �Ʒ��� �շ������� �ش� ���� �ڳʰ� �ȴ�.
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

		// ���� �Ʒ� �밢�� üũ�� ���� �� ��尡 �ڳʰ� �ƴ϶�� ���� ����� �Ʒ��� ������ üũ�Ͽ�
		// �ڳʰ� �ִ����� �Ǵ��Ѵ�.
		NavNode* FindNode = GetRectNodeBottom(CheckNode, EndNode, End);

		// ���� �˻��� ��带 ã�Ҵٸ� ������ ��带 �ڳʷ� üũ�Ѵ�.
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
	// �������� �̵��Ҷ��� ���ΰ� 1�� �����ϸ� ���� �����ְ� ���� ���� �� �� �ְų�
	// �Ʒ��� �����ְ� ���� �Ʒ��� �� �� ���� ��� �ڳʰ� �ȴ�.
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
	// ���� ���� �̵��Ҷ��� ��带 ��ĭ�� ���� ���� �̵��� ��Ű�鼭 �ش� ����� �Ʒ����� �����ְ� ���� �Ʒ��� �շ��ְų�
	// �������� �����ְ� ������ ���� �շ������� �ش� ���� �ڳʰ� �ȴ�.
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

		// ���� �� �밢�� üũ�� ���� �� ��尡 �ڳʰ� �ƴ϶�� ���� ����� ���� ������ üũ�Ͽ�
		// �ڳʰ� �ִ����� �Ǵ��Ѵ�.
		NavNode* FindNode = GetRectNodeTop(CheckNode, EndNode, End);

		// ���� �˻��� ��带 ã�Ҵٸ� ������ ��带 �ڳʷ� üũ�Ѵ�.
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

	// ���� ���� ?

	if (SrcNode->Total > DestNode->Total)
		return -1;
	else if (SrcNode->Total < DestNode->Total)
		return 1;
	return 0;
 }
