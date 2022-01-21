#include "LoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/ViewPort.h"

CLoadingScene::CLoadingScene()
{}

CLoadingScene::~CLoadingScene()
{}

bool CLoadingScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_LoadingWidget = m_Scene->GetViewPort()->CreateUIWindow<CLoadingWidget>("LoadingWidget");

	return true;
}
