#pragma once

#include "../GameInfo.h"
#include "../Resource/Animation/AnimationSequence2D.h"

struct Animation2DNotify
{
	std::string           Name;
	int                   Frame;
	bool                  Call;
	std::function<void()> Function;

	Animation2DNotify() :
		Frame(0),
		Call(false)
	{
	}
};

class CAnimationSequence2DData
{
	friend class CAnimationSequence2DInstance;

private:
	CAnimationSequence2DData();
	~CAnimationSequence2DData();
private:
	std::string                            m_Name;
	std::string							   m_SequenceName;
	CSharedPtr<class CAnimationSequence2D> m_Sequence;
	int                                       m_Frame;     // 현재 애니메이션 프레임
	float                                  m_Time;      // 애니메이션 동작 시간
	float                                  m_FrameTime; // 1프레임당 시간
	float                                  m_PlayTime;
	float                                  m_PlayScale; // 재생 비율
	bool                                   m_Loop;
	bool                                   m_Reverse;
	std::function<void()>                  m_EndFunction;
	std::vector<Animation2DNotify*>        m_vecNotify;
public :
	bool IsFrameReverse() const
{
		return m_Sequence->IsFrameReverse();
}
	bool IsLoop() const
{
		return m_Loop;
}
	bool IsReverse() const
{
		return m_Reverse;
}
	std::string GetName() const
{
		return m_Name;
}
	float GetPlayTime () const
{
		return m_PlayTime;
}
	int GetCurrentFrame() const
	{
		return m_Frame;
	}
	int GetFrameCount() const
	{
		return m_Sequence->GetFrameCount();
	}

	AnimationFrameData GetCurrentFrameData() const
	{
		return m_Sequence->GetFrameData(m_Frame);
	}

	CAnimationSequence2D* GetAnimationSequence() const
	{
		return m_Sequence;
	}

	AnimationFrameData GetFrameData(int Frame)
	{
		return m_Sequence->GetFrameData(Frame);
	}

public :
	void ClearSequenceFrame()
{
		m_Sequence->ClearFrame();
}
	void AddFrame(const Vector2& StartPos, const Vector2& Size)
	{
		m_Sequence->AddFrame(StartPos, Size);
	}

	void AddFrame(float StartX, float StartY, float SizeX, float SizeY)
	{
		m_Sequence->AddFrame(StartX, StartY, SizeX, SizeY);
	}

	void SetFrameData(const Vector2& StartPos, const Vector2& Size, int Index = 0)
{
		if (m_Sequence->GetFrameCount() <= 0)
			return;
		m_Sequence->SetFrame(Index, StartPos, Size);
}

	void SetLoop(bool State)
{
		m_Loop = State;
}
	void SetReverse(bool State)
{
		m_Reverse = State;
}
	void SetFrame(int Frame)
{
		m_Frame = 0;
}
	void ResetFrame()
	{
		m_Frame = 0;
		m_Sequence->ClearFrame();
	}
	void SetPlayScale(float Scale)
{
		m_PlayScale = Scale;
}
	void SetPlayTime(float Time)
	{
		m_PlayTime = Time;
	}
	void SetFrameReverse(bool Reverse)
{
		m_Sequence->SetFrameReverse(Reverse);
}
	void SetSequence2D(CAnimationSequence2D* Sequence2D);
	void SetSequenceTexture(class CTexture* Texture);
public :
	bool Save(FILE* pFile);
	bool Load(FILE* pFile);
public:
	template <typename T>
	void SetEndFunction(T* Obj, void (T::*Func)())
	{
		m_EndFunction = std::bind(Func, Obj);
	}

	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void (T::*Func)())
	{
		Animation2DNotify* Notify = new Animation2DNotify;

		Notify->Name     = Name;
		Notify->Frame    = Frame;
		Notify->Function = std::bind(Func, Obj);

		m_vecNotify.push_back(Notify);
	}
};
