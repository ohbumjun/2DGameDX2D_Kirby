#include "EditorMenu.h"
#include "IMGUITextInput.h"
#include "IMGUIComboBox.h"
#include "IMGUISameLine.h"
#include "IMGUIButton.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../EditorManager.h"
#include "ObjectHierarchy.h"
#include "IMGUIListBox.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"


CEditorMenu::CEditorMenu()
{
}

CEditorMenu::~CEditorMenu()
{
}

bool CEditorMenu::Init()
{
	CIMGUIWindow::Init();

	m_ObjectComboBox = AddWidget<CIMGUIComboBox>("ObjectComboBox", 150.f, 30.f);
	m_ObjectComboBox->AddItem("GameObject");
	m_ObjectComboBox->AddItem("Player");
	m_ObjectComboBox->SetHideName(true);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("ObjectName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectButton = AddWidget<CIMGUIButton>("CreateObject", 120.f, 30.f);
	m_ObjectButton->SetClickCallback(this, &CEditorMenu::CreateNewObject);

	m_ComponentComboBox = AddWidget<CIMGUIComboBox>("ComponentComboBox", 150.f, 30.f);
	m_ComponentComboBox->AddItem("SpriteComponent");
	m_ComponentComboBox->AddItem("SceneComponent");
	m_ComponentComboBox->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentButton = AddWidget<CIMGUIButton>("CreateComponent", 120.f, 30.f);
	m_ComponentButton->SetClickCallback(this, &CEditorMenu::CreateNewComponent);

	return true;
}

void CEditorMenu::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CEditorMenu::CreateNewObject()
{
	// Object Type 선택
	if (m_ObjectComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ObjectNameInput->Empty())
		return;

	switch ((CreateObject_Type)m_ObjectComboBox->GetSelectIndex())
	{
	case CreateObject_Type::GameObject :
		{
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	case CreateObject_Type::Player :
		{
			
		}
		break;
	}

	// Add List To Object List
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	Hierarchy->AddObject(m_ObjectNameInput->GetTextUTF8());
}

void CEditorMenu::CreateNewComponent()
{
	// Component Type 선택
	if (m_ComponentComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ComponentNameInput->Empty())
		return;

	// Object 가 ObjectHierarcy 에서 선택되어 있어야 한다
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	// 해당 Object 의 Component 목록에 추가
	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	switch ((CreateComponent_Type)m_ComponentComboBox->GetSelectIndex())
	{
	case CreateComponent_Type::Sprite :
		{
		// #include "Component/SpriteComponent.h"
		// #include "Component/StaticMeshComponent.h"s
		CSpriteComponent* SpriteComponent = new CSpriteComponent;
		SpriteComponent->SetName(m_ComponentNameInput->GetTextMultibyte());
		Object->AddSceneComponent(SpriteComponent);
		}
		break;
	case CreateComponent_Type::Static:
		{
		CStaticMeshComponent* StaticMeshComponent = new CStaticMeshComponent;
		StaticMeshComponent->SetName(m_ComponentNameInput->GetTextMultibyte());
		Object->AddSceneComponent(StaticMeshComponent);
		}
		break;
	}

	// Add Item To Component List Box
	Hierarchy->GetComponentListBox()->AddItem(m_ComponentNameInput->GetTextMultibyte());
}
