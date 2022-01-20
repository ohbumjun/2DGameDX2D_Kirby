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
#include "../Object/SpriteEditObject.h"
#include "../Object/Player2D.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "PathManager.h"
#include "Engine.h"
#include "../Object/ShowObject.h"


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

	// ========================================================================

	m_ComponentComboBox = AddWidget<CIMGUIComboBox>("ComponentComboBox", 150.f, 30.f);
	m_ComponentComboBox->AddItem("SpriteComponent");
	m_ComponentComboBox->AddItem("StaticComponent");
	m_ComponentComboBox->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 80.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentButton = AddWidget<CIMGUIButton>("CreateComponent", 120.f, 30.f);
	m_ComponentButton->SetClickCallback(this, &CEditorMenu::CreateNewComponent);

	// ========================================================================

	m_SaveSceneButton = AddWidget<CIMGUIButton>("SaveScene", 80.f, 30.f);
	m_SaveSceneButton->SetClickCallback(this, &CEditorMenu::SaveScene);

	Line = AddWidget<CIMGUISameLine>("Line");
	m_LoadSceneButton = AddWidget<CIMGUIButton>("LoadScene", 80.f, 30.f);
	m_LoadSceneButton->SetClickCallback(this, &CEditorMenu::LoadScene);

	// ========================================================================

	m_PlayButton = AddWidget<CIMGUIButton>("Play", 80.f, 30.f);
	m_PlayButton->SetClickCallback(this, &CEditorMenu::TogglePlay);


	return true;
}

void CEditorMenu::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CEditorMenu::CreateNewObject()
{
	// Object Type ����
	if (m_ObjectComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ObjectNameInput->Empty())
		return;

	// �ߺ� ����
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	const std::string NewObjectName = m_ObjectNameInput->GetTextMultibyte();
	if (Hierarchy->GetObjectListBox()->CheckItem(NewObjectName))
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
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPlayer2D>(m_ObjectNameInput->GetTextMultibyte());
		}
		break;
	}

	// Add List To Object List
	Hierarchy->AddObject(m_ObjectNameInput->GetTextUTF8());

}

void CEditorMenu::CreateNewComponent()
{
	// Component Type ����
	if (m_ComponentComboBox->GetSelectIndex() < 0)
		return;

	// Text Input
	if (m_ComponentNameInput->Empty())
		return;

	// Object �� ObjectHierarcy ���� ���õǾ� �־�� �Ѵ�
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	// �ߺ� ����
	const std::string NewComponentName = m_ComponentNameInput->GetTextMultibyte();
	if (Hierarchy->GetComponentListBox()->CheckItem(NewComponentName))
		return;

	// �ش� Object �� Component ��Ͽ� �߰�
	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	switch ((CreateComponent_Type)m_ComponentComboBox->GetSelectIndex())
	{
	case CreateComponent_Type::Sprite :
		{
		Object->CreateComponent<CSpriteComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	case CreateComponent_Type::Static:
		{
		Object->CreateComponent<CStaticMeshComponent>(m_ComponentNameInput->GetTextMultibyte());
		}
		break;
	}

	// Add Item To Component List Box
	Hierarchy->GetComponentListBox()->AddItem(m_ComponentNameInput->GetTextMultibyte());
}

void CEditorMenu::TogglePlay()
{
	if (CEngine::GetInst()->IsPlay())
		CEngine::GetInst()->SetPlay(false);
	else
		CEngine::GetInst()->SetPlay(true);

}

void CEditorMenu::SaveScene()
{
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (!Hierarchy || Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	TCHAR FileFullPath[MAX_PATH] = {};
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = FileFullPath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0*.Scene File\0*.scn");
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		CSceneManager::GetInst()->GetScene()->SaveFullPath(FilePathMultibyte);
	}
}

void CEditorMenu::LoadScene()
{
	// ������ �۾����� ������ �ִٸ�, ��, ObjectHierarchy�� GameObject, Sprite ����� �����ִٸ�
	// Message Box ���� return;
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();
	if (Hierarchy->GetObjectListBox()->GetItemCount() > 0)
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Complete Current Work"), TEXT("Data Might Be Lost"), 0);
		return;
	}

	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0*.Scene File\0*.scn");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;
	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);
		CSceneManager::GetInst()->GetScene()->LoadFullPath(FilePathMultibyte);

		// Scene�� Object ����� ���鼭, Object Hierarchy �� Add ��Ű�� ����
		// Object ���� �̸��� �����´�
		std::vector<std::string> vecObjNames;
		CSceneManager::GetInst()->GetScene()->GatherObjectsNames(vecObjNames);

		CIMGUIListBox* ObjectListBox = Hierarchy->GetObjectListBox();

		size_t NameCount = vecObjNames.size();
		for (size_t i = 0; i < NameCount; i++)
		{
			ObjectListBox->AddItem(vecObjNames[i]);
		}

		// ȭ�鿡 Scene Edit Object�� �����
		CEditorManager::GetInst()->SetSceneEditObject();


		// ���� ó�� �༮�� Component ���� �����صд�.
		Hierarchy->SelectObject(0, vecObjNames[0].c_str());

		// ���� ���õ� Component�� ��ġ�� �����Ѵ�.
		CShowObject* ShowObject = CEditorManager::GetInst()->GetSceneEditObject();

		std::string ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem();
		CGameObject* SelectedObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName.c_str());

		Vector3 ObjectPivot = SelectedObject->GetPivot();
		Vector3 ObjectSize = SelectedObject->GetWorldScale();
		Vector3 ObjectPos = SelectedObject->GetWorldPos();
		Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

		Vector2 StartPos = Vector2(Pos.x, Pos.y);
		Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

		ShowObject->SetStartPos(StartPos);
		ShowObject->SetEndPos(EndPos);
	}
}
