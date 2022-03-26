#pragma once
#include "Scene\SceneMode.h"

class CFloat5Scene :
    public CSceneMode
{
public:
	CFloat5Scene();
	virtual ~CFloat5Scene() override;
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
};

