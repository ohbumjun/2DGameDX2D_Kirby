#include "ObjectHierarchy.h"
#include "IMGUIManager.h"
#include "TileMapWindow.h"
#include "IMGUISameLine.h"
#include "IMGUIListBox.h"
#include "IMGUILabel.h"
#include "GameObject/GameObject.h"
#include "../Object/ShowObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../EditorManager.h"
#include "DetailInfoWindow.h"
#include "Component/TileMapComponent.h"
#include "Component/TileEmptyComponent.h"


CObjectHierarchy::CObjectHierarchy()
{
}

CObjectHierarchy::~CObjectHierarchy()
{
}

bool CObjectHierarchy::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("ObjectList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("ComponentList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	m_ObjectListBox = AddWidget<CIMGUIListBox>("ObjectList", 200.f, 400.f);
	m_ObjectListBox->SetHideName(true);
	m_ObjectListBox->SetPageItemCount(20);
	m_ObjectListBox->SetSelectCallback(this, &CObjectHierarchy::SelectObject);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentListBox = AddWidget<CIMGUIListBox>("ComponentList", 200.f, 400.f);
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
	m_SelectObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// ���� ��� �����ֱ�
	m_ComponentListBox->Clear();

	// ���� ���� ���� ���� Root Component �� ���̴� 
	std::vector<FindComponentName> vecComponentsName;
	m_SelectObject->GetAllSceneComponentsName(vecComponentsName);

	// �ڱ� �ڽ��� ������ ���·� �д�.
	m_ObjectListBox->SetSelectIndex(Index);

	for (size_t i = 0; i < vecComponentsName.size(); i++)
	{
		m_ComponentListBox->AddItem(vecComponentsName[i].Name);
	}

	// ���� ù��° Component�� ������ ���·� �����Ѵ�.
	m_ComponentListBox->SetSelectIndex(0);

	// Object �� Pos, Rot, Scale x,y,z ������ DetailWindow ���� �������ش�.
	CDetailInfoWindow* DetailWindow = CEditorManager::GetInst()->GetDetailWindow();
	DetailWindow->SetPosRotScaleInfo(m_SelectObject);

	CEditorManager::GetInst()->SetSceneEditObject();

	// ȭ�鿡 ShowObject ��ġ�� Object�� Root Component ������ ����
	CShowObject* ShowObject = CEditorManager::GetInst()->GetShowObject();
	
	Vector3 ObjectPivot = m_SelectObject->GetPivot();
	Vector3 ObjectSize = m_SelectObject->GetWorldScale();
	Vector3 ObjectPos = m_SelectObject->GetWorldPos();
	Vector3 Pos = ObjectPos - ObjectPivot * ObjectSize;

	Vector2 StartPos = Vector2(Pos.x, Pos.y);
	Vector2 EndPos = Vector2(Pos.x + ObjectSize.x, Pos.y + ObjectSize.y);

	ShowObject->SetStartPos(StartPos);
	ShowObject->SetEndPos(EndPos);
}

void CObjectHierarchy::SelectComponent(int Index, const char* ComponentName)
{
	if (!m_SelectObject)
		return;

	m_SelectComponent = (CSceneComponent*)m_SelectObject->FindComponent(ComponentName);
	// TileMapWindow

	CTileMapWindow* TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");

	if (TileMapWindow && m_SelectComponent->CheckType<CTileEmptyComponent>())
	{
		// TileMapWindow->SetTileMap((CTileEmptyComponent*)m_SelectComponent);
		TileMapWindow->SetTileMap((CTileEmptyComponent*)m_SelectComponent);

		// Edit Mode�� Tile Edit ���� �ٲ��ش�
		CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
	}
	else
	{
		TileMapWindow->SetTileMap(nullptr);
	}

	/*
	if (TileMapWindow && m_SelectComponent->CheckType<CTileMapComponent>())
	{
		TileMapWindow->SetTileMap((CTileMapComponent*)m_SelectComponent);

		// Edit Mode�� Tile Edit ���� �ٲ��ش�
		CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
	}
	else
	{
		TileMapWindow->SetTileMap(nullptr);
	}
	*/
}
