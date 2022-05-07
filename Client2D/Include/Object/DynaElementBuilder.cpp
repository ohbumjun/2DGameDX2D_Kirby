#include "DynaElementBuilder.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CBossDynaBaby* CDynaBabyBuilder::CreateBaby(const std::string& Name)
{
	m_CurrentDynaBaby = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBossDynaBaby>("Attack");

	CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("BossDynaBabyMake");

	// m_BabiesList.push_back(DynaBaby);

	return m_CurrentDynaBaby;

	float NumberFrom0To1 = (float)rand() / (float)RAND_MAX;

	// 오른쪽 2개
	for (int i = 0; i < 2; i++)
	{
		float XPos = i & 1 ? GetWorldPos().x + NumberFrom0To1 * 100.f : GetWorldPos().x - NumberFrom0To1 * 100.f;

		CBossDynaBaby* DynaBaby = m_Scene->CreateGameObject<CBossDynaBaby>("Attack");

		DynaBaby->SetWorldPos(XPos, GetWorldPos().y, 0.f);

		DynaBaby->m_ObjectMoveDir.x = i & 1 ? 1.f : -1.f;
		// AttackEffect->SetRightAttackDir(0.f);

		DynaBaby->m_JumpVelocity = 40.f + NumberFrom0To1 * 10.f;

		DynaBaby->JumpStart();

		DynaBaby->SetWorldScale(70.f, 70.f, 1.f);

		DynaBaby->m_BossDyna = m_BossDyna;

		// Boss Dyna 의 Baby List 에 추가 
		m_BossDyna->AddBossDynaBaby(DynaBaby);
	}

}

CBossDynaBaby* CDynaBabyBuilder::SetWorldPos(float x, float y, float z)
{
	m_CurrentDynaBaby->SetWorldPos(x, y, z);
	return m_CurrentDynaBaby;
}


CBossDynaBaby* CDynaBabyBuilder::SetWorldScale(float x, float y, float z)
{
	m_CurrentDynaBaby->SetWorldScale(x, y, z);
	return m_CurrentDynaBaby;
}

CBossDynaBaby* CDynaBabyBuilder::SetObjectMoveDir(float Dir)
{
	m_CurrentDynaBaby->m_ObjectMoveDir = Dir;
	return m_CurrentDynaBaby;
}

CBossDynaBaby* CDynaBabyBuilder::SetJumpVelocity(float Velocity)
{
	m_CurrentDynaBaby->m_JumpVelocity = Velocity;
	return m_CurrentDynaBaby;
}

CDynaNest*   CDynaNestBuilder::CreateNest()
{
	return m_Nest = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDynaNest>("DynaNest");
}

CDynaNest* CDynaNestBuilder::SetWorldPos(float x, float y, float z)
{
	m_Nest->SetWorldPos(x, y, z);
	return m_Nest;
}
