#include "EditorMenu.h"

#include <IMGUITextInput.h>

#include "IMGUIComboBox.h"
#include "IMGUISameLine.h"
#include "IMGUIButton.h"

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
	m_ObjectComboBox->SetSelectCallback(this, &CEditorMenu::SelectObjectType);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("ObjectName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("CreateObject", 120.f, 30.f);
	Button->SetClickCallback(this, &CEditorMenu::CreateNewObject);

	/*
	class CIMGUIComboBox* m_ObjectComboBox;
    class CIMGUITextInput* m_ObjectNameInput;
	class CIMGUIComboBox* m_ComponentComboBox;
    class CIMGUITextInput* m_ComponentNameInput;
	 */

	m_ComponentComboBox = AddWidget<CIMGUIComboBox>("ComponentComboBox", 150.f, 30.f);
	m_ComponentComboBox->AddItem("SpriteComponent");
	m_ComponentComboBox->AddItem("SceneComponent");
	m_ComponentComboBox->SetHideName(true);
	m_ComponentComboBox->SetSelectCallback(this, &CEditorMenu::SelectComponentType);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Button = AddWidget<CIMGUIButton>("CreateComponent", 120.f, 30.f);
	Button->SetClickCallback(this, &CEditorMenu::CreateNewObject);

	return true;
}

void CEditorMenu::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CEditorMenu::SelectObjectType(int Index, const char* ObjectType)
{
}

void CEditorMenu::SelectComponentType(int Index, const char* ComponentType)
{
}

void CEditorMenu::CreateNewObject()
{
}

void CEditorMenu::CreateNewComponent()
{
}
