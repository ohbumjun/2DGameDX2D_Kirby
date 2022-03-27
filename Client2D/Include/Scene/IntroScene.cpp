#include "IntroScene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/Monster.h"
#include "../Object/TileMap.h"
#include "../Object/TileMapEmpty.h"
#include "../Object/PixelTest.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "../UI/IntroWidget.h"
#include "../Object/LineContainer.h"

CIntroScene::CIntroScene()
{
	SetTypeID<CIntroScene>();
}

CIntroScene::~CIntroScene()
{}

void CIntroScene::Start()
{
}

bool CIntroScene::Init()
{
	if (!CSceneMode::Init())
		return false;

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroWidget>("IntroWidget");

	return true;
}

void CIntroScene::CreateMaterial()
{
	CSceneMode::CreateMaterial();
}

void CIntroScene::CreateSound()
{}

void CIntroScene::CreateParticle()
{}

void CIntroScene::PrepareResources()
{
	CSceneMode::PrepareResources();

	CreateMaterial();

	CreateParticle();

	CreateSound();
}
