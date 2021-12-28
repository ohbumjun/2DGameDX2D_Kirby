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
		fwrite(&TypeID, sizeof(size_t), 1, pFile); // 
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
	if (!m_RootComponent)
		return;
	m_RootComponent->GetAllSceneComponentsName(vecNames);
}
