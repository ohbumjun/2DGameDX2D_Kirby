#pragma once

#include "Scene/SceneMode.h"

class CMainScene :
	public CSceneMode
{
public:
	CMainScene();
	virtual ~CMainScene() override;

public:
	virtual bool Init() override;

private:
	void CreateMaterial();
	void CreateAnimationSequence();
};
