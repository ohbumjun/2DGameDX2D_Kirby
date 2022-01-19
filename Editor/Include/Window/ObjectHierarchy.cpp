#include "ObjectHierarchy.h"
#include "IMGUISameLine.h"
#include "IMGUIListBox.h"
#include "IMGUILabel.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/ResourceManager.h"
#include "../EditorManager.h"
#include "DetailInfoWindow.h"

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
	// 선택한 Object 내에 있는 Scene Component 목록을 보여준다
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// 기존 모두 지워주기
	m_ComponentListBox->Clear();

	// 가장 먼저 들어온 놈이 Root Component 일 것이다 
	std::vector<FindComponentName> vecComponentsName;
	Object->GetAllSceneComponentsName(vecComponentsName);

	// 자기 자신을 선택한 상태로 둔다.
	m_ObjectListBox->SetSelectIndex(Index);

	for (size_t i = 0; i < vecComponentsName.size(); i++)
	{
		m_ComponentListBox->AddItem(vecComponentsName[i].Name);
	}

	// 가장 첫번째 Component를 선택한 상태로 세팅한다.
	m_ComponentListBox->SetSelectIndex(0);

	// Drag Object 들의 x,y,z 정보도 세팅해준다.
	CDetailInfoWindow* DetailWindow = CEditorManager::GetInst()->GetDetailWindow();
	DetailWindow->SetPosRotScaleInfo(Object);
}

void CObjectHierarchy::SelectComponent(int Index, const char* ComponentName)
{
}
