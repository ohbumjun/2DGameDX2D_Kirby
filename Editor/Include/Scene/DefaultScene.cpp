#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CDefaultScene::CDefaultScene()
{
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdle.sqc");
	// m_Scene->GetResource()->LoadSequence2D("TestNew.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRun.sqc");

	return true;
}
