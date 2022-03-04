#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CDefaultScene::CDefaultScene()
{
	SetTypeID<CDefaultScene>();

	m_IsEditMode = true;
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
	CreateMaterial();

	m_Scene->GetResource()->LoadSequence2D("PlayerIdle.sqc");
	// m_Scene->GetResource()->LoadSequence2D("TestNew.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRun.sqc");

	return true;
}

void CDefaultScene::CreateMaterial()
{
	m_Scene->GetResource()->CreateMaterial<CMaterial>("TileMapMaterial");
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("TileMapMaterial");

	Material->SetShader("TileMapShader");
	Material->SetRenderState("AlphaBlend");
}
