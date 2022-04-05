#pragma once
#include "Scene\SceneMode.h"

class CEndingScene :
    public CSceneMode
{
public:
	CEndingScene();
	virtual ~CEndingScene() override;
private:
	class CUIWindow* m_MainWidget;
public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
private:
	void PrepareResources() override;
	void CreateSound();
};

