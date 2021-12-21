#pragma once

#include "AnimationSequence2D.h"

class CAnimationManager
{
	friend class CResourceManager;

private:
	CAnimationManager();
	~CAnimationManager();

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	class CAnimation2DConstantBuffer*                                 m_Animation2DCBuffer;

public:
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const
	{
		return m_Animation2DCBuffer;
	}

public:
	bool Init();
	bool CreateAnimationSequence2D(const std::string& Name, const std::string&     TextureName,
	                               const TCHAR*       FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& Name, CTexture* Texture);
	void AddFrame(const std::string& Name, const Vector2& Start, const Vector2& Size);
	void AddFrame(const std::string& Name, float StartX, float StartY, float Width, float Height);

	CAnimationSequence2D* FindSequence(const std::string& Name);
	void                  ReleaseSequence(const std::string& Name);
	bool LoadSequence2D(std::string& SequenceName, const char* FullPath, const class CScene* Scene);
};
