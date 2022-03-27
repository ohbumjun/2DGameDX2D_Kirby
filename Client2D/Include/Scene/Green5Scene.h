#pragma once
#include "Scene\SceneMode.h"

class CGreen5Scene :
    public CSceneMode
{
public:
	CGreen5Scene();
	virtual ~CGreen5Scene() override;
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
};

