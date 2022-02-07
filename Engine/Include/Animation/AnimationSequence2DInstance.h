#pragma once

#include "AnimationSequence2DData.h"

class CAnimationSequence2DInstance
{
	friend class CSpriteComponent;
	friend class CSpriteEditWindow;

public:
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Anim);
	virtual ~CAnimationSequence2DInstance();
protected:
	size_t m_TypeID;
	std::string m_Name;
	bool                                                       m_PlayAnimation;
	class CSpriteComponent*                                    m_Owner;
	class CScene*                                              m_Scene;
	std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimation;
	CAnimationSequence2DData*                                  m_CurrentAnimation;
	class CAnimation2DConstantBuffer*                          m_CBuffer;
public :
	void Stop()
	{
		m_PlayAnimation = false;
	}
	void Play()
	{
		m_PlayAnimation = true;
	}
	bool IsPlay() const
	{
		return m_PlayAnimation == true;
	}
	int GetAnimationCount() const
	{
		return static_cast<int>(m_mapAnimation.size());
	}
	size_t GetTypeID() const
	{
		return m_TypeID;
	}
	bool IsFrameReverse() const
	{
		if (!m_CurrentAnimation)
			return false;
		return m_CurrentAnimation->IsFrameReverse();
	}
	CAnimationSequence2DData* GetCurrentAnimation() const;
public :
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}
	void SetOwner(class CSpriteComponent* Owner)
	{
		m_Owner = Owner;
	}
	void SetFrameReverse(bool Reverse)
{
		if (!m_CurrentAnimation)
			return;
		m_CurrentAnimation->SetFrameReverse(Reverse);
}
public :
	int GetCurrentAnimationOrder();
	void GatherSequenceNames(std::vector<std::string>& vecNames);
	void AddSequence2DToScene();
	void ClearAnimationSequence();
	bool DeleteAnimationSequence(const std::string& Name);
	void EditSequence2DName(const std::string& PrevName, const std::string& NewName);
	void AdjustSequence2DKeyName();
public:
	void SetFrameData(const Vector2& StartPos, const Vector2& Size,int Index = 0);
	void AddAnimation(const std::string& AnimationName, CAnimationSequence2DData* Animation);
	void AddAnimation(const std::string& AnimationName, const std::string& SequenceName,
		CAnimationSequence2D* Sequence2D, bool Loop = true, float  PlayTime = 1.f,
		float PlayScale = 1.f, bool Reverse = false);
	void AddAnimation(const std::string& SequenceName, const std::string& AnimationName, bool Loop = true, 
		float PlayTime = 1.f,
	                  float              PlayScale                                        = 1.f, bool Reverse = false);
	void AddAnimation(const TCHAR* FileName, const std::string& PathName, 
		const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float              PlayScale = 1.f, bool Reverse = false);
	void                      SetPlayTime(const std::string& Name, float PlayTime);
	void                      SetPlayScale(const std::string& Name, float PlayScale);
	void                      SetReverse(const std::string& Name, bool Reverse);
	void                      SetLoop(const std::string& Name, bool Loop);
	void                      SetCurrentAnimation(const std::string& Name);
	void                      ChangeAnimation(const std::string& Name);
	bool                      CheckCurrentAnimation(const std::string& Name);
	CAnimationSequence2DData* FindAnimationSequence2DData(const std::string& Name);

public:
	virtual void                          Start();
	virtual bool                          Init();
	virtual void                          Update(float DeltaTime);
	void                                  SetShader();
	void                                  ResetShader();
	virtual CAnimationSequence2DInstance* Clone();
public :
	bool                      SaveFullPath(const char* FullPath);
	bool						LoadFullPath(const char* FullPath);
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void (T::*Func)())
	{
		CAnimationSequence2DData* Data = FindAnimationSequence2DData(Name);

		if (!Data)
			return;

		Data->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, const std::string& NotifyName, int Frame,
	               T*                 Obj, void (T::*          Func)())
	{
		CAnimationSequence2DData* Data = FindAnimationSequence2DData(Name);

		if (!Data)
			return;

		Data->AddNotify<T>(NotifyName, Frame, Obj, Func);
	}
	template<typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}
};
