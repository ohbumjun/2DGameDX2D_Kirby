#include "DynaElementBuilder.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CBossDynaBaby* CDynaBabyBuilder::CreateBaby(const std::string& Name)
{
	m_CurrentDynaBaby = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBossDynaBaby>("Attack");

	CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("BossDynaBabyMake");

	m_CurrentDynaBaby->JumpStart();

	// m_BabiesList.push_back(DynaBaby);

	return m_CurrentDynaBaby;
}

CDynaBabyBuilder* CDynaBabyBuilder::SetWorldPos(float x, float y, float z)
{
	m_CurrentDynaBaby->SetWorldPos(x, y, z);
	return this;
}

CDynaBabyBuilder* CDynaBabyBuilder::SetWorldScale(float x, float y, float z)
{
	m_CurrentDynaBaby->SetWorldScale(x, y, z);
	return this;
}

CDynaBabyBuilder* CDynaBabyBuilder::SetObjectMoveDir(float Dir)
{
	m_CurrentDynaBaby->m_ObjectMoveDir = Dir;
	return this;
}

CDynaBabyBuilder* CDynaBabyBuilder::SetJumpVelocity(float Velocity)
{
	m_CurrentDynaBaby->m_JumpVelocity = Velocity;
	return this;
}

void CDynaBabyBuilder::DeleteDynaBaby(CBossDynaBaby* Baby)
{
	auto iter = m_BabiesList.begin();
	auto iterEnd = m_BabiesList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Baby)
		{
			m_BabiesList.erase(iter);
			return;
		}
	}
}


// Nest Builder
CDynaNest*   CDynaNestBuilder::CreateNest()
{
	return m_Nest = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDynaNest>("DynaNest");
}

CDynaNest* CDynaNestBuilder::SetWorldPos(float x, float y, float z)
{
	m_Nest->SetWorldPos(x, y, z);
	return m_Nest;
}
