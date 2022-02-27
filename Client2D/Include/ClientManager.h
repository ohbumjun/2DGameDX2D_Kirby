#pragma once

#include "GameInfo.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int  Run();

	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimationInstance(class CSpriteComponent* Component, const size_t AnimationTypeID);


	DECLARE_SINGLE(CClientManager)
};
