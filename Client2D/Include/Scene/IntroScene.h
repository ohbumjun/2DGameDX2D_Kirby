#pragma once
#include "Scene\SceneMode.h"

class CIntroScene :
    public CSceneMode
{
public:
	CIntroScene();
	virtual ~CIntroScene() override;
private:
	class CUIWindow* m_MainWidget;
public:
	virtual void Start() override;
	virtual bool Init() override;
private:
	void CreateMaterial();
	void CreateSound();
	void CreateParticle();
public:
	virtual void PrepareResources() override;
};

