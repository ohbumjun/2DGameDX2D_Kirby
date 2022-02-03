#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"

CStartScene::CStartScene()
{}

CStartScene::~CStartScene()
{}

bool CStartScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	// m_StartWidget = m_Scene->GetViewPort()->CreateUIWindow<CStartWidget>("StartWidget");

	return true;
}
