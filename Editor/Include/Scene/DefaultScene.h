#pragma once
#include "Scene/SceneMode.h"

class CDefaultScene :
	public CSceneMode
{
public:
	CDefaultScene();
	virtual ~CDefaultScene() override;

public:
	virtual bool Init() override;
};
