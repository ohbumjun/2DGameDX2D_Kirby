#pragma once
#include "Scene\SceneMode.h"

class CBeginningScene :
    public CSceneMode
{
public:
	CBeginningScene();
	virtual ~CBeginningScene() override;
private:
	class CUIWindow* m_MainWidget;
	std::function<void(bool, float)> m_LoadingFunction;
public:
	virtual void Start() override;
	virtual bool Init() override;

private:
	void CreateMaterial();
	void CreateAnimationSequence();
	void CreateSound();
	void CreateParticle();
public:
	virtual void PrepareResources() override;
public:
	template<typename T>
	void SetLoadingFunction(T* Obj, void (T::* Func)(bool, float))
	{
		m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
};

