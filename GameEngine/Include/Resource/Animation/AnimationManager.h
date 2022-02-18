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
	std::unordered_map<std::string, class CAnimationSequence2DInstance*> m_mapSequenceInstance;
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

	CAnimationSequence2D* FindSequence2D(const std::string& Name);
	void                  ReleaseSequence(const std::string& Name);
	bool AddSequence2D(CAnimationSequence2D* Sequence2D);
	bool AddSequence2D(const std::string& SequenceName, CTexture* Texture);
	bool LoadSequence2DFullPath(std::string& SequenceName, const char* FullPath, const class CScene* Scene);
	bool LoadSequence2D(std::string& SequenceName, const char* FileName, const std::string& PathName, const class CScene* Scene);
	void EditSequence2DName(const std::string& PrevName, const std::string& NewName);
	class CAnimationSequence2DInstance* LoadAnimationInstance(const std::string& Name, 
		const TCHAR* FileName, const std::string& PathName = ENGINE_ANIMATION_PATH);
	class CAnimationSequence2DInstance* FindAnimationInstance(const std::string& Name);
	void ReleaseAnimationInstance(const std::string& Name);
};
