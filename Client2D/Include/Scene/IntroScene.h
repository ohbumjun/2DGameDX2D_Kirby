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
	virtual bool Init() override;
private:
	void CreateSound();
};

