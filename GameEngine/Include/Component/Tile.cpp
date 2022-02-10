#include "Tile.h"

CTile::CTile() :
	m_TileShape(Tile_Shape::Rect),
	m_TileType(Tile_Type::Normal),
	m_AnimInstance(nullptr),
	m_Owner(nullptr),
	m_IndexX(-1),
	m_IndexY(-1),
	m_Index(-1)
{
}

CTile::CTile(const CTile& tile)
{
	*this = tile;

	m_AnimInstance = tile.m_AnimInstance->Clone();
}

CTile::~CTile()
{
	SAFE_DELETE(m_AnimInstance);
}

void CTile::AddAnimation(const std::string& SequenceName, const std::string& Name, bool Loop,
	float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_AnimInstance)
		m_AnimInstance = new CAnimationSequence2DInstance;

	m_AnimInstance->AddAnimation(SequenceName, Name, Loop, PlayTime, PlayScale, Reverse);
}

void CTile::AddAnimation(const TCHAR* FileName, const std::string& PathName, const std::string& Name,
	bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_AnimInstance)
		m_AnimInstance = new CAnimationSequence2DInstance;

	m_AnimInstance->AddAnimation(FileName, PathName, Name, Loop, PlayTime, PlayScale, Reverse);
}

void CTile::SetPlayTime(const std::string& Name, float PlayTime)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetPlayTime(Name, PlayTime);
}

void CTile::SetPlayScale(const std::string& Name, float PlayScale)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetPlayScale(Name, PlayScale);
}

void CTile::SetReverse(const std::string& Name, bool Reverse)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetReverse(Name, Reverse);
}

void CTile::SetLoop(const std::string& Name, bool Loop)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetLoop(Name, Loop);
}

void CTile::SetCurrentAnimation(const std::string& Name)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetCurrentAnimation(Name);
}

void CTile::ChangeAnimation(const std::string& Name)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->ChangeAnimation(Name);
}

bool CTile::CheckCurrentAnimation(const std::string& Name)
{
	if (!m_AnimInstance)
		return false;

	return m_AnimInstance->CheckCurrentAnimation(Name);
}

void CTile::Start()
{
	m_Center = m_Pos + (m_Size / 2.f);
}

void CTile::Update(float DeltaTime)
{
	if (m_AnimInstance)
		m_AnimInstance->Update(DeltaTime);
}
