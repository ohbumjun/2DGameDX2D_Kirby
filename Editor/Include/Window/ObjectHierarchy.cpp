#include "ObjectHierarchy.h"
#include "IMGUISameLine.h"
#include "IMGUIListBox.h"
#include "IMGUILabel.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/ResourceManager.h"

CObjectHierarchy::CObjectHierarchy()
{
}

CObjectHierarchy::~CObjectHierarchy()
{
}

bool CObjectHierarchy::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("ObjectList", 300.f, 30.f);
	Label->SetColor(0, 0, 255);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("ComponentList", 300.f, 30.f);
	Label->SetColor(0, 0, 255);

	m_ObjectListBox = AddWidget<CIMGUIListBox>("ObjectList", 300.f, 500.f);
	m_ObjectListBox->SetHideName(true);
	m_ObjectListBox->SetPageItemCount(20);
	m_ObjectListBox->SetSelectCallback(this, &CObjectHierarchy::SelectObject);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentListBox = AddWidget<CIMGUIListBox>("ComponentList", 300.f, 500.f);
	m_ComponentListBox->SetHideName(true);
	m_ComponentListBox->SetPageItemCount(20);
	m_ComponentListBox->SetSelectCallback(this, &CObjectHierarchy::SelectComponent);

	return true;
}

void CObjectHierarchy::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CObjectHierarchy::AddObject(const char* ObjectName)
{
	m_ObjectListBox->AddItem(ObjectName);
}

void CObjectHierarchy::SelectObject(int Index, const char* ObjectName)
{
	// ������ Object ���� �ִ� Scene Component ����� �����ش�
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// ���� ��� �����ֱ�
	m_ComponentListBox->Clear();

	std::vector<FindComponentName> vecComponentsName;
	Object->GetAllSceneComponentsName(vecComponentsName);
	for (size_t i = 0; i < vecComponentsName.size(); i++)
	{
		m_ComponentListBox->AddItem(vecComponentsName[i].Name);
	}
}

void CObjectHierarchy::SelectComponent(int Index, const char* ComponentName)
{
}
