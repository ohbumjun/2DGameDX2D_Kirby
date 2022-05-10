#include "DynaElementBuilder.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CDynaBabyBuilder::CDynaBabyBuilder()
{
	m_BossDynaBabyPool = new CSimplePoolAllocator<CBossDynaBaby>(50);
}

CDynaBabyBuilder::~CDynaBabyBuilder()
{
	SAFE_DELETE(m_BossDynaBabyPool);
}

CBossDynaBaby* CDynaBabyBuilder::CreateBaby(const std::string& Name)
{
	
	m_CurrentDynaBaby = m_BossDynaBabyPool->allocate();
	m_CurrentDynaBaby->SetName(Name);
	m_CurrentDynaBaby->SetScene(CSceneManager::GetInst()->GetScene());

	if (!m_CurrentDynaBaby->Init())
	{
		m_BossDynaBabyPool->deallocate(m_CurrentDynaBaby);
		return nullptr;
	}

	if (CSceneManager::GetInst()->GetScene()->IsStart())
		m_CurrentDynaBaby->Start();

	// m_CurrentDynaBaby = CSceneManager::GetInst()->GetScene()->CreateGameObject<CBossDynaBaby>(Name);

	m_CurrentDynaBaby->JumpStart();

	m_BabiesList.push_back(m_CurrentDynaBaby);

	// Scene 의 Object List 에 추가하기
	CSceneManager::GetInst()->GetScene()->AddObjectToList(m_CurrentDynaBaby);

	// Delete Call back Function 세팅하기
	m_CurrentDynaBaby->SetDeleteCallback<CDynaBabyBuilder>(this, &CDynaBabyBuilder::DeAllocate);

	CSceneManager::GetInst()->GetScene()->GetResource()->SoundPlay("BossDynaBabyMake");

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

void CDynaBabyBuilder::DestroyAllBabies()
{
	auto iter = m_BabiesList.begin();
	auto iterEnd = m_BabiesList.end();

	for (; iter != iterEnd;)
	{
		(*iter)->Destroy();
		iter = m_BabiesList.erase(iter);
	}
}

void CDynaBabyBuilder::DeAllocate(CRef* CurrentDynaBaby)
{
	m_BossDynaBabyPool->deallocate(dynamic_cast<CBossDynaBaby*>(CurrentDynaBaby));
}

void CDynaBabyBuilder::Update(float DeltaTime)
{
	auto iter = m_BabiesList.begin();
	auto iterEnd = m_BabiesList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			m_BossDynaBabyPool->deallocate((*iter));
			iter = m_BabiesList.erase(iter);
			iterEnd = m_BabiesList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->Update(DeltaTime);
		++iter;
	}
}

void CDynaBabyBuilder::PostUpdate(float DeltaTime)
{
	auto iter = m_BabiesList.begin();
	auto iterEnd = m_BabiesList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			m_BossDynaBabyPool->deallocate((*iter));
			iter = m_BabiesList.erase(iter);
			iterEnd = m_BabiesList.end();
			continue;
		}
		if (!(*iter)->IsEnable())
			continue;
		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

void CDynaBabyBuilder::Render()
{}

// Nest Builder
CDynaNest*   CDynaNestBuilder::CreateNest()
{
	// Nest Builder 는 Memory Pool 관리 X
	return m_Nest = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDynaNest>("DynaNest");
}

CDynaNest* CDynaNestBuilder::SetWorldPos(float x, float y, float z)
{
	m_Nest->SetWorldPos(x, y, z);
	return m_Nest;
}
