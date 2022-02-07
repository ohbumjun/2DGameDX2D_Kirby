#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"
#include "../Texture/Texture.h"

CAnimationManager::CAnimationManager() :
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
                                                  const std::string& TextureName, const TCHAR* FileName,
                                                  const std::string& PathName)
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

bool CAnimationManager::CreateAnimationSequence2D(const std::string& Name, CTexture* Texture)
{
	CAnimationSequence2D* Sequence = FindSequence(Name);

	if (Sequence)
		return true;

	Sequence = new CAnimationSequence2D;
	Sequence->SetName(Name);

	if (!Sequence->Init(Texture))
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

void CAnimationManager::AddFrame(const std::string& Name, float  StartX, float StartY,
                                 float              Width, float Height)
{
	CAnimationSequence2D* Sequence = FindSequence(Name);

	if (!Sequence)
		return;

	Sequence->AddFrame(StartX, StartY, Width, Height);
}

CAnimationSequence2D* CAnimationManager::FindSequence(const std::string& Name)
{
	auto iter = m_mapSequence2D.find(Name);

	if (iter == m_mapSequence2D.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseSequence(const std::string& Name)
{
	auto iter = m_mapSequence2D.find(Name);

	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence2D.erase(iter);
	}
}

bool CAnimationManager::AddSequence2D(CAnimationSequence2D* Sequence2D)
{
	CAnimationSequence2D* Anim = FindSequence(Sequence2D->GetName());

	if (Anim)
		return true;;

	// 여기서 ResourceManager 에 까지 넣어줄 필요는 없는 것일까 ?

	m_mapSequence2D.insert(std::make_pair(Sequence2D->GetName(), Sequence2D));
	return true;
}

bool CAnimationManager::AddSequence2D(const std::string& SequenceName, CTexture* Texture)
{
	CAnimationSequence2D* Sequence2D = FindSequence(SequenceName);
	if (Sequence2D)
		return true;

	Sequence2D = new  CAnimationSequence2D;
	Sequence2D->m_Texture = Texture;

	m_mapSequence2D.insert(std::make_pair(SequenceName, Sequence2D));

	return true;
}

bool CAnimationManager::LoadSequence2DFullPath(std::string& SequenceName, const char* FullPath, const CScene* Scene)
{
	CAnimationSequence2D* Sequence2D = new  CAnimationSequence2D;
	Sequence2D->SetScene(const_cast<CScene*>(Scene));
	if (!Sequence2D->LoadFullPath(FullPath))
	{
		SAFE_DELETE(Sequence2D);
		return false;
	}
	SequenceName = Sequence2D->GetName();

	if (FindSequence(SequenceName))
	{
		SAFE_RELEASE(Sequence2D);
		return true;
	}

	m_mapSequence2D.insert(std::make_pair(SequenceName, Sequence2D));
	return true;
}

bool CAnimationManager::LoadSequence2D(std::string& SequenceName, const char* FileName, const std::string& PathName,
                                       const CScene* Scene)
{
	CAnimationSequence2D* Sequence2D = new CAnimationSequence2D;
	Sequence2D->SetScene(const_cast<CScene*>(Scene));

	if (!Sequence2D->Load(FileName, PathName))
	{
		SAFE_DELETE(Sequence2D);
		return false;
	}

	SequenceName = Sequence2D->GetName();

	if (FindSequence(SequenceName))
	{
		SAFE_RELEASE(Sequence2D);
		return true;
	}

	SequenceName = Sequence2D->GetName();

	m_mapSequence2D.insert(std::make_pair(SequenceName, Sequence2D));
	return true;
}

void CAnimationManager::EditSequence2DName(const std::string& PrevName, const std::string& NewName)
{
	auto iter = m_mapSequence2D.find(PrevName);

	if (iter == m_mapSequence2D.end())
		return;

	CAnimationSequence2D* Sequence2D = iter->second;
	if (!Sequence2D)
		return;

	CAnimationSequence2D* NewSequence2D = Sequence2D->Clone();
	NewSequence2D->SetName(NewName); // 실제 Sequence의 이름도 바꿔준다.

	m_mapSequence2D.erase(iter);

	m_mapSequence2D.insert(std::make_pair(NewName, Sequence2D));
}
