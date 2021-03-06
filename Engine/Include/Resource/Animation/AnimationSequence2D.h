#pragma once

#include "../../Ref.h"

class CAnimationSequence2D :
	public CRef
{
	friend class CAnimationManager;
	friend class CSceneResource;
	friend class CAnimationSequence2DData;

public:
	CAnimationSequence2D();
	CAnimationSequence2D(const CAnimationSequence2D& Sequence2D);
	virtual ~CAnimationSequence2D() override;

private:
	class CScene*                   m_Scene;
	CSharedPtr<class CTexture>      m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;
	bool m_IsFrameReverse;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	bool IsFrameReverse() const
	{
		return m_IsFrameReverse;
	}
	class CTexture* GetTexture() const
	{
		return m_Texture;
	}

	const std::vector<AnimationFrameData>& GetVectorFrameData() const
	{
		return m_vecFrameData;
	}

	const AnimationFrameData& GetFrameData(int Index) const
	{
		return m_vecFrameData[Index];
	}

	int GetFrameCount() const
	{
		return static_cast<int>(m_vecFrameData.size());
	}

public :
	void SetFrameReverse(bool Reverse)
	{
		m_IsFrameReverse = Reverse;
	}
	void ClearFrame()
	{
		m_vecFrameData.clear();
	}

	void DeleteFrame(int Index)
	{
		// m_vecFrameData.erase(m_vecFrameData.begin(), m_vecFrameData.begin() + Index);
		m_vecFrameData.erase(m_vecFrameData.begin() + Index);
	}

public:
	bool Init(class CTexture* Texture);
	bool Init(const std::string& Name, const TCHAR* FileName,
	          const std::string& PathName = TEXTURE_PATH);
	void AddFrame(const Vector2& StartPos, const Vector2& Size);
	void AddFrame(float StartX, float StartY, float Width, float Height);
	void SetFrame(int Index, float StartX, float StartY, float Width, float Height);
	void SetFrame(int Index, const Vector2& StartPos, const Vector2& Size);
	CAnimationSequence2D* Clone();
public :
	void SaveFullPath(const char* FullPath);
	void Save(FILE* pFile);
	bool LoadFullPath(const char* FullPath);
	bool Load(const char* FileName, const std::string& PathName);
	bool Load(FILE* pFile);
};
