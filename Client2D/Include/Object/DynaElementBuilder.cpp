#include "DynaElementBuilder.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CDynaNest* CDynaNestBuilder::CreateNest()
{
	return m_Nest = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDynaNest>("DynaNest");
}

CDynaNest* CDynaNestBuilder::SetWorldPos(float x, float y, float z)
{
	m_Nest->SetWorldPos(x, y, z);
	return m_Nest;
}
