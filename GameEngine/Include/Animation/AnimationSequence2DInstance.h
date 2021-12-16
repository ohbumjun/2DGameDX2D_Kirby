#pragma once

#include "AnimationSequence2DData.h"

class CAnimationSequence2DInstance
{
	friend class CSpriteComponent;

public:
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& Anim);
	virtual ~CAnimationSequence2DInstance();

protected:
	bool m_PlayAnimation;
	class CSpriteComponent* m_Owner;
	class CScene* m_Scene;
	std::unordered_map<std::string, CAnimationSequence2DData*>	m_mapAnimation;
	CAnimationSequence2DData* m_CurrentAnimation;
	class CAnimation2DConstantBuffer* m_CBuffer;

public:
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}
	void Stop()
	{
		m_PlayAnimation = false;
	}
	void Play()
	{
		m_PlayAnimation = true;
	}

	void SetOwner(class CSpriteComponent* Owner)
	{
		m_Owner = Owner;
	}

	int GetAnimationCount()	const
	{
		return (int)m_mapAnimation.size();
	}

public:
	void AddAnimation(const std::string& SequenceName, const std::string& Name, bool Loop = true, float PlayTime = 1.f,
		float PlayScale = 1.f, bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetReverse(const std::string& Name, bool Reverse);
	void SetLoop(const std::string& Name, bool Loop);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void SetShader();
	void ResetShader();
	virtual CAnimationSequence2DInstance* Clone();

private:
	CAnimationSequence2DData* FindAnimation(const std::string& Name);

public:
	template <typename T>
	void SetEndFunction(const std::string& Name, T* Obj, void (T::* Func)())
	{
		CAnimationSequence2DData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddNotify(const std::string& Name, const std::string& NotifyName, int Frame,
		T* Obj, void (T::* Func)())
	{
		CAnimationSequence2DData* Data = FindAnimation(Name);

		if (!Data)
			return;

		Data->AddNotify<T>(NotifyName, Frame, Obj, Func);
	}
};

