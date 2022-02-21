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

	// ���� ��ã�� ������ ����, ����ߴ� ������ ���� ���� �����Ƿ�
	// ���� ã�� ���� ����ϴ� ������ �����͸� �ʱ�ȭ ���ش�
	 size_t Size = m_vecUseNode.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecUseNode[i]->NodeType = Nav_Node_Type::None;
		m_vecUseNode[i]->Cost = FLT_MAX;
		m_vecUseNode[i]->Dist = FLT_MAX;
		m_vecUseNode[i]->Total = FLT_MAX;
		m_vecUseNode[i]->Parent = nullptr;
	}

	// ���� ���, ��ǥ ��� ���ϱ�
	NavNode* StartNode = m_vecNode[StartIndex];
	NavNode* EndNode = m_vecNode[EndIndex];

	// ��ǥ ��尡 Ȥ�ó� ���̶�� false
	if (EndNode->TileType == Tile_Type::Wall)
		return false;

	// ��� ���, ����尡 �����ϴٸ� ���� ó��
	else if (StartNode == EndNode)
	{
		vecPath.push_back(End);
		return true;
	}

	// ���� ��带 ���� ��Ͽ� ������Ų��.
	StartNode->NodeType = Nav_Node_Type::Open;

	m_vecOpen.push_back(StartNode);

	// ���� ��带 ���� �����ӿ��� ������ ��� ��Ͽ� �߰����ش�.
	m_vecUseNode.push_back(StartNode);

	while (!m_vecOpen.empty())
	{
		NavNode* Node = m_vecOpen.front();

		m_vecOpen.pop_back();

		// �ش� ����� ���� ������� �ٲ��ش�. --> �湮 ó���� ����
		Node->NodeType = Nav_Node_Type::Close;

		// �ش� ��带 �������� �ֺ� 8���� Ž���� �Ѵ�.

		// ���� ����� �����Ѵ�. --> ����� ���� ��尡 ���� ������ ��尡 �ǵ��� ������������ �����Ѵ�.
		if (!m_vecOpen.empty())
		{
			std::sort(m_vecUseNode.begin(), m_vecUseNode.end(), SortNode);
		}
	}

	 return false;
 }

bool CNavigation::FindNode(NavNode* Node, NavNode* EndNode,  const Vector3& End, std::vector<Vector3>& vecPath)
 {
	// �ֺ� 8������ ���鼭, �ڳʸ� ã�´�.
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
