#include "SceneComponent.h"
#include "../Engine.h"
#include "../GameObject/GameObject.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	m_Transform = new CTransform;

	m_Transform->m_Owner = this;
	m_Transform->Init();

	m_Parent = nullptr;

	m_LayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com) :
	CComponent(com)
{
	*this = com;
	m_Transform = com.m_Transform->Clone();

	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	m_Transform->m_Owner = this;

	m_Parent = nullptr;

	m_vecChild.clear();

	size_t Size = com.m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* CloneComponent = com.m_vecChild[i]->Clone();

		CloneComponent->m_Parent = this;

		m_vecChild.push_back(CloneComponent);

		CloneComponent->m_Transform->m_Parent = m_Transform;

		m_Transform->m_vecChild.push_back(CloneComponent->m_Transform);
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::SetSceneComponent(CGameObject* Object)
{
	Object->AddSceneComponent(this);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetSceneComponent(Object);
	}
}

void CSceneComponent::GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames)
{
	FindComponentName Name;
	Name.Name = m_Name;
	/*
	if (m_Parent)
		Name.ParentName = m_Parent->GetName();
	*/
	vecNames.push_back(Name);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->GetAllSceneComponentsName(vecNames);
	}
}

void CSceneComponent::ReAddTransformChild()
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->m_Transform->m_Parent = m_Transform;

		m_Transform->m_vecChild.push_back(m_vecChild[i]->m_Transform);
	}
}

void CSceneComponent::SetScene(CScene* Scene)
{
	CComponent::SetScene(Scene);

	m_Transform->m_Scene = Scene;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetScene(Scene);
	}
}

void CSceneComponent::SetGameObject(CGameObject* Object)
{
	CComponent::SetGameObject(Object);

	m_Transform->m_Object = Object;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetGameObject(Object);
	}
}

void CSceneComponent::AddChild(CSceneComponent* Child)
{
	Child->m_Parent = this;

	m_vecChild.push_back(Child);

	Child->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(Child->m_Transform);
}

bool CSceneComponent::DeleteChild(CSceneComponent* Child)
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChild[i] == Child)
		{
			auto iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}

		if (m_vecChild[i]->DeleteChild(Child))
			return true;
	}

	return false;
}

bool CSceneComponent::DeleteChild(const std::string& Name)
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChild[i]->GetName() == Name)
		{
			auto iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}

		if (m_vecChild[i]->DeleteChild(Name))
			return true;
	}

	return false;
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& Name)
{
	if (m_Name == Name)
		return this;

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* Find = m_vecChild[i]->FindComponent(Name);

		if (Find)
			return Find;
	}

	return nullptr;
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->Destroy();
	}
}

void CSceneComponent::Start()
{
	CComponent::Start();

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Start();
	}
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(float DeltaTime)
{
	m_Transform->Update(DeltaTime);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Update(DeltaTime);
	}
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
	m_Transform->PostUpdate(DeltaTime);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PostUpdate(DeltaTime);
	}
}

void CSceneComponent::PrevRender()
{
	if (m_Render)
		CRenderManager::GetInst()->AddRenderList(this);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PrevRender();
	}
}

void CSceneComponent::Render()
{
	m_Transform->SetTransform();

	CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
	CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
}

void CSceneComponent::CheckCollision()
{
	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecChild[i]->CheckCollision();
	}

}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);
	/*
	CSceneComponent*                         m_Parent; --> Load �Ǵ� �������� �˾Ƽ� ���õ� ���̴�.
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild; -->
	bool                                     m_Render;
	 */
	int Length = (int)m_LayerName.length();
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_LayerName.c_str(), sizeof(char), Length, pFile);

	m_Transform->Save(pFile);

	int ChildCount = (int)m_vecChild.size();
	fwrite(&ChildCount, sizeof(int), 1, pFile);
	for (int i = 0; i < ChildCount; i++)
	{
		size_t TypeID = m_vecChild[i]->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, pFile);
		m_vecChild[i]->Save(pFile);
	}

	fwrite(&m_Render, sizeof(bool), 1, pFile);
}

void CSceneComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);
	/*
	CSceneComponent*                         m_Parent; --> Load �Ǵ� �������� �˾Ƽ� ���õ� ���̴�.
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild; -->
	 */
	int Length;
	fread(&Length, sizeof(int), 1, pFile);

	char LayerName[MAX_PATH] = {};
	fread(LayerName, sizeof(char), Length, pFile);
	m_LayerName = LayerName;

	m_Transform->Load(pFile);

	int ChildCount;
	fread(&ChildCount, sizeof(int), 1, pFile);

	size_t TypeID;
	for (int i = 0; i < ChildCount; i++)
	{
		fread(&TypeID, sizeof(size_t), 1, pFile);
		CComponent* Component = CSceneManager::GetInst()->CallCreateComponentFunc(m_Object, TypeID);
		Component->Load(pFile);

		CSceneComponent* ChildSceneComponent = dynamic_cast<CSceneComponent*>(Component);
		ChildSceneComponent->m_Parent = this;

		m_vecChild.push_back(ChildSceneComponent);

		ChildSceneComponent->m_Transform->m_Parent = m_Transform;

		m_Transform->m_vecChild.push_back(ChildSceneComponent->m_Transform);
	}

	fread(&m_Render, sizeof(bool), 1, pFile);
}
