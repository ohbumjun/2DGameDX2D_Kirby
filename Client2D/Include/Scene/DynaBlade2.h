#pragma once
#include "Scene\SceneMode.h"

class CDynaBlade2 :
    public CSceneMode
{
public:
	CDynaBlade2();
	virtual ~CDynaBlade2() override;
private:
	class CUIWindow* m_MainWidget;
	class CUIWindow* m_BossWidget;
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

