#include "Tile.h"

#include "TileMapComponent.h"

CTile::CTile() :
	m_Shape(Tile_Shape::Rect),
	m_TileType(Tile_Type::Normal),
	m_AnimInstance(nullptr),
	m_Owner(nullptr),
	m_IndexX(-1),
	m_IndexY(-1),
	m_Index(-1),
	m_Opacity(1.f)
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

void CTile::SetFrameStart(float x, float y)
{
	m_FrameStart = Vector2(x, y);
}

void CTile::SetFrameEnd(float x, float y)
{
	m_FrameEnd = Vector2(x, y);
}

void CTile::AddAnimation(const std::string& SequenceName, const std::string& Name, bool      Loop,
						 float              PlayTime, float                  PlayScale, bool Reverse)
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

// Update 함수는 출력을 위한 준비를 하는 함수이다.
void CTile::Update(float DeltaTime)
{
	if (m_AnimInstance)
	{
		// Frame Update
		m_AnimInstance->Update(DeltaTime);

		int Frame = m_AnimInstance->GetCurrentAnimation()->GetCurrentFrame();

		m_FrameStart = m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(Frame).Start;
		m_FrameEnd  = m_FrameEnd + m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(Frame).Size;
		
	}

	Vector3 WorlsPos = m_Owner->GetWorldPos() + m_Pos;

	Matrix matScale, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);

	matTranslate.Translation(WorlsPos);

	m_matWorld = matScale * matTranslate;
}
