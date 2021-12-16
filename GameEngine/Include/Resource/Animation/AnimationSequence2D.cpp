
#include "AnimationSequence2D.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"

CAnimationSequence2D::CAnimationSequence2D()	:
	m_Scene(nullptr)
{
}

CAnimationSequence2D::~CAnimationSequence2D()
{
}

bool CAnimationSequence2D::Init(CTexture* Texture)
{
	m_Texture = Texture;

	return true;
}

bool CAnimationSequence2D::Init(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return false;

		m_Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	else
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return false;

		m_Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	return true;
}

void CAnimationSequence2D::AddFrame(const Vector2& StartPos, const Vector2& Size)
{
	AnimationFrameData	Data = {};

	Data.Start = StartPos;
	Data.Size = Size;

	m_vecFrameData.push_back(Data);
}

void CAnimationSequence2D::AddFrame(float StartX, float StartY, float Width, float Height)
{
	AnimationFrameData	Data = {};

	Data.Start = Vector2(StartX, StartY);
	Data.Size = Vector2(Width, Height);

	m_vecFrameData.push_back(Data);
};

