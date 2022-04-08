#include "IntroScene.h"
#include "../Object/Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/Monster.h"
#include "Scene/ViewPort.h"
#include "../UI/MainWidget.h"
#include "../UI/IntroWidget.h"
#include "../Object/LineContainer.h"

CIntroScene::CIntroScene()
{
	SetTypeID<CIntroScene>();
}

CIntroScene::~CIntroScene()
{
	// SAFE_DELETE(m_MainWidget);
}

bool CIntroScene::Init()
{
	if (!CSceneMode::Init()) //
		return false;

	CreateSound();

	// m_Scene->GetResource()->CreateSoundGroup();
	m_Scene->GetResource()->LoadSound("BGM", "IntroBGM", true, TEXT("BG_Intro.mp3"));
	m_Scene->GetResource()->SoundPlay("IntroBGM");

	m_MainWidget = m_Scene->GetViewPort()->CreateUIWindow<CIntroWidget>("IntroWidget");

	return true;
}

void CIntroScene::CreateSound()
{
	CSceneResource* Resource = m_Scene->GetResource();
	// m_Scene->GetResource()->LoadSound("BGM", "IntroBGM", true, TEXT("BG_Intro.mp3"));
	// m_Scene->GetResource()->SoundPlay("IntroBGM");

}
