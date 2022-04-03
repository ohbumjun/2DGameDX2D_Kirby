#pragma once
#include "Scene\SceneMode.h"

class CDynaBlade1 :
    public CSceneMode
{
public:
	CDynaBlade1();
	virtual ~CDynaBlade1() override;
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

