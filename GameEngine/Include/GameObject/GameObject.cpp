#include "GameObject.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

CGameObject::CGameObject() :
	m_Scene(nullptr),
	m_Parent(nullptr)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;

	m_RefCount = 0;

	if (obj.m_RootComponent)
	{
		m_RootComponent = obj.m_RootComponent->Clone();

		m_RootComponent->SetGameObject(this);

		m_RootComponent->SetSceneComponent(this);
	}

	m_vecObjectComponent.clear();

	size_t Size = obj.m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent.push_back(obj.m_vecObjectComponent[i]->Clone());

		m_vecObjectComponent[i]->SetGameObject(this);
	}
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}

CComponent* CGameObject::FindComponent(const std::string& Name)
{
	{
		auto iter    = m_SceneComponentList.begin();
		auto iterEnd = m_SceneComponentList.end();
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}
	}

	{
		auto iter    = m_vecObjectComponent.begin();
		auto iterEnd = m_vecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}
	}

	return nullptr;
}

void CGameObject::Destroy()
{
	CRef::Destroy();
	if (m_RootComponent)
		m_RootComponent->Destroy();

	size_t Size = m_vecObjectComponent.size();
	for (size_t i = 0; i < Size; i++)
	{
		m_vecObjectComponent[i]->Destroy();
	}

}

void CGameObject::ClearSceneComponents()
{
	// SceneComponent 에서 Root Component 정보를 Release 해주고
	// Root Component를 null로 세팅
	auto iter = m_SceneComponentList.begin();
	auto iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd ;++iter)
	{
		if ((*iter)->GetName() == m_RootComponent->GetName())
		{
			m_SceneComponentList.erase(iter);
			m_RootComponent = nullptr;
			break;
		}
	}

	// 그 다음 SceneComponent 목록들을 돌면서 Release 시켜준다.
	iter = m_SceneComponentList.begin();
	iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((* iter));
	}
	m_SceneComponentList.clear(); // ?
}

void CGameObject::DeleteComponent(const std::string& Name)
{
	if (!m_RootComponent)
		return;

	// 만약 Root Component 라면 X
	if (m_RootComponent->GetName() == Name)
		return;

	// Root Component를 통해 해당 Component 를 지워준다.
	bool Delete = m_RootComponent->DeleteChild(Name);
	if (Delete)
		return;

	// Root Component 에도 속하지 않을 경우를 대비해서, SceneComponent 전체를 돌면서 Erase를 실행한다
	auto iter      = m_SceneComponentList.begin();
	auto iterEnd = m_SceneComponentList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->DeleteChild(Name))
		{
			Delete = true;
			break;
		}
	}

	// 전체 SceneComponent 목록에서 해당 Component 정보를 지워준다
	iter = m_SceneComponentList.begin();
	iterEnd = m_SceneComponentList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			iter = m_SceneComponentList.erase(iter);
			break;
		}
	}
}

void CGameObject::Start()
{
	if (m_RootComponent)
		m_RootComponent->Start();

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Start();
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->Update(DeltaTime);
}

void CGameObject::PostUpdate(float DeltaTime)
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->PostUpdate(DeltaTime);
}

void CGameObject::PrevRender()
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PrevRender();
	}

	if (m_RootComponent)
		m_RootComponent->PrevRender();
}

void CGameObject::Render()
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Render();
	}

	if (m_RootComponent)
		m_RootComponent->Render();
}

void CGameObject::PostRender()
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostRender();
	}

	if (m_RootComponent)
		m_RootComponent->PostRender();
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::AddCollision()
{
	if (m_RootComponent)
		m_RootComponent->CheckCollision();
}

void CGameObject::Save(FILE* pFile)
{
	CRef::Save(pFile);

	bool Root = false;
	if (m_RootComponent)
		Root = true;

	fwrite(&Root, sizeof(bool), 1, pFile);

	if (Root)
	{
		size_t TypeID = m_RootComponent->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, pFile); 
		m_RootComponent->Save(pFile);
	}

	int ObjectCount = (int)m_vecObjectComponent.size();
	fwrite(&ObjectCount, sizeof(int), 1, pFile);

	for (int i = 0; i < ObjectCount; i++)
	{
		size_t TypeID = m_vecObjectComponent[i]->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, pFile);
		m_vecObjectComponent[i]->Save(pFile);
	}

	/*
	아래 목록들으 Load 하는 과정에서 자연스럽게 추가할 예정이다.
	-------------------------------------------------------------------------
	std::list<CSceneComponent*>               m_SceneComponentList;
	CGameObject*                         m_Parent;
	std::vector<CSharedPtr<CGameObject>> m_vecChildObject;
	 */
}

void CGameObject::Load(FILE* pFile)
{
	CRef::Load(pFile);

	bool Root = false;
	fread(&Root, sizeof(bool), 1, pFile);

	size_t TypeID;
	if (Root)
	{
		fread(&TypeID, sizeof(size_t), 1, pFile);
		CSceneManager::GetInst()->CallCreateComponentFunc(this, TypeID);
		m_RootComponent->Load(pFile);
	}

	int ObjectCount;
	fread(&ObjectCount, sizeof(int), 1, pFile);

	for (int i = 0; i < ObjectCount; i++)
	{
		fread(&TypeID, sizeof(size_t), 1, pFile);
		CComponent* Component = CSceneManager::GetInst()->CallCreateComponentFunc(this, TypeID);
		Component->Load(pFile);
		m_vecObjectComponent.push_back(dynamic_cast<CObjectComponent*>(Component));
	}
}

void CGameObject::GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames)
{
	/*
	if (!m_RootComponent)
		return;
	m_RootComponent->GetAllSceneComponentsName(vecNames);
	*/
	auto iter = m_SceneComponentList.begin();
	auto iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->GetAllSceneComponentsName(vecNames);
	}

}
