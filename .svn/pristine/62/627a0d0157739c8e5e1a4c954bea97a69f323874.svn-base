
#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"

CAnimationManager::CAnimationManager()	:
	m_Animation2DCBuffer(nullptr)
{
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_Animation2DCBuffer);
}

bool CAnimationManager::Init()
{
	m_Animation2DCBuffer = new CAnimation2DConstantBuffer;

	m_Animation2DCBuffer->Init();

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& Name, 
	const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence2D* Sequence = FindSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;

	Sequence->SetName(Name);

	if (!Sequence->Init(TextureName, FileName, PathName))
	{
		SAFE_DELETE(Sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(Name, Sequence));

	return true;
}

void CAnimationManager::AddFrame(const std::string& Name, const Vector2& Start, const Vector2& Size)
{
	CAnimationSequence2D* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->AddFrame(Start, Size);
}

void CAnimationManager::AddFrame(const std::string& Name, float StartX, float StartY, 
	float Width, float Height)
{
	CAnimationSequence2D* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->AddFrame(StartX, StartY, Width, Height);
}

CAnimationSequence2D* CAnimationManager::FindSequence(const std::string& Name)
{
	auto	iter = m_mapSequence2D.find(Name);

	if (iter == m_mapSequence2D.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseSequence(const std::string& Name)
{
	auto	iter = m_mapSequence2D.find(Name);

	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence2D.erase(iter);
	}
}
