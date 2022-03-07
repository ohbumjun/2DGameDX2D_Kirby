#include "ObjectHierarchy.h"
#include "BackGroundWindow.h"
#include "IMGUIManager.h"
#include "TileMapWindow.h"
#include "IMGUISameLine.h"
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
#include "Component/BackGroundComponent.h"


CObjectHierarchy::CObjectHierarchy()
{
}

CObjectHierarchy::~CObjectHierarchy()
{
}

bool CObjectHierarchy::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("CreatedObjectList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("CreatedComponentList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	// ========================================================================

	m_CreatedObjectListBox = AddWidget<CIMGUIListBox>("CreatedObjectList", 200.f, 200.f);
	m_CreatedObjectListBox->SetHideName(true);
	m_CreatedObjectListBox->SetPageItemCount(20);
	m_CreatedObjectListBox->SetSelectCallback(this, &CObjectHierarchy::SelectCreatedObject);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_CreatedComponentListBox = AddWidget<CIMGUIListBox>("CreatedComponentList", 200.f, 200.f);
	m_CreatedComponentListBox->SetHideName(true);
	m_CreatedComponentListBox->SetPageItemCount(20);
	m_CreatedComponentListBox->SetSelectCallback(this, &CObjectHierarchy::SelectCreatedComponent);


	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ========================================================================

	Label = AddWidget<CIMGUILabel>("ObjectList", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Specific Object List", 200.f, 30.f);
	Label->SetColor(0, 0, 255);

	// ========================================================================

	m_ObjectList = AddWidget<CIMGUIListBox>("ObjectList", 200.f, 200.f);
	m_ObjectList->SetHideName(true);
	m_ObjectList->SetPageItemCount(20);
	m_ObjectList->SetSelectCallback(this, &CObjectHierarchy::SeeObjectList);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_SpecificObjectList = AddWidget<CIMGUIListBox>("Specific Object List", 200.f, 200.f);
	m_SpecificObjectList->SetHideName(true);
	m_SpecificObjectList->SetPageItemCount(20);
	m_SpecificObjectList->SetSelectCallback(this, &CObjectHierarchy::SeeSpecificObjectList);

	// ========================================================================

	// m_mapVecObject ����
	std::vector<std::string>  vecNormalMonsters = { g_NormalBearName, g_PurpleBeatlesName, g_YellowBirdName };

	m_mapVecObject.insert(std::make_pair(g_NormalBearName, vecNormalMonsters));

	// m_mapVecObject �� Key ���� m_ObjectList �� �߰�
	for (const auto & kv : m_mapVecObject)
	{
		m_ObjectList->AddItem(kv.first);
	}

	// Normal Monster �� m_ObjectList ���� ���õ� ���·� �α�
	m_ObjectList->SetSelectIndex(0);

	// m_SpecificObjectList �� Normal Monster �� ���� ���� �ϱ�
	for (size_t i = 0; i < vecNormalMonsters.size(); i++)
	{
		m_SpecificObjectList->AddItem(vecNormalMonsters[i]);
	}

	return true;
}

void CObjectHierarchy::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CObjectHierarchy::SetSeletedObjectByName(const std::string& Name)
{
	int Index = m_CreatedObjectListBox->FindItemIndex(Name);

	if (Index == -1)
		return;

	m_CreatedObjectListBox->SetSelectIndex(Index);
}

void CObjectHierarchy::AddCreatedObject(const char* ObjectName)
{
	m_CreatedObjectListBox->AddItem(ObjectName);
}

void CObjectHierarchy::SelectCreatedObject(int Index, const char* ObjectName)
{
	// ������ Object ���� �ִ� Scene Component ����� �����ش�
	m_SelectObject = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// ���� ��� �����ֱ�
	m_CreatedComponentListBox->Clear();

	// ���� ���� ���� ���� Root Component �� ���̴� 
	std::vector<FindComponentName> vecComponentsName;
	m_SelectObject->GetAllSceneComponentsName(vecComponentsName);

	// �ڱ� �ڽ��� ������ ���·� �д�.
	m_CreatedObjectListBox->SetSelectIndex(Index);

	for (size_t i = 0; i < vecComponentsName.size(); i++)
	{
		m_CreatedComponentListBox->AddItem(vecComponentsName[i].Name);
	}

	// ���� ù��° Component�� ������ ���·� �����Ѵ�.
	m_CreatedComponentListBox->SetSelectIndex(0);

	// TileMap Ȥ�� BackGround �� �ƴ϶�� 
	// Object �� Pos, Rot, Scale x,y,z ������ DetailWindow ���� �������ش�.
	if (!m_SelectObject->GetRootComponent()->CheckType<CTileEmptyComponent>() &&
		!m_SelectObject->GetRootComponent()->CheckType<CBackGroundComponent>())
	{
		CDetailInfoWindow* DetailWindow = CEditorManager::GetInst()->GetDetailWindow();
		DetailWindow->SetClickedObjectInfo(m_SelectObject->GetRootComponent());

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
}

void CObjectHierarchy::SeeObjectList(int Index, const char* ObjectName)
{
	m_SpecificObjectList->Clear();

	std::vector<std::string> vecObjectLists = m_mapVecObject[ObjectName];
	for (size_t i = 0; i < vecObjectLists.size(); i++)
	{
		m_SpecificObjectList->AddItem(vecObjectLists[i]);
	}
}

void CObjectHierarchy::SeeSpecificObjectList(int Index, const char* ComponentName)
{
	// ���⼭�� ȭ�鿡 
}

bool CObjectHierarchy::CheckDuplicateObject(const std::string& ObjName)
{
	return m_CreatedObjectListBox->CheckItem(ObjName);
}

void CObjectHierarchy::SelectCreatedComponent(int Index, const char* ComponentName)
{
	if (!m_SelectObject)
		return;

	m_SelectComponent = (CSceneComponent*)m_SelectObject->FindComponent(ComponentName);

	CTileMapWindow* TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");

	CBackGroundWindow* BackGroundWindow = (CBackGroundWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("BackGroundWindow");

	// Tile Map Component ��� 
	if (TileMapWindow && m_SelectComponent->CheckType<CTileEmptyComponent>())
	{
		// TileMapWindow->SetTileMap((CTileEmptyComponent*)m_SelectComponent);
		TileMapWindow->SetTileMap((CTileEmptyComponent*)m_SelectComponent);

		// Edit Mode�� Tile Edit ���� �ٲ��ش�
		CEditorManager::GetInst()->SetEditMode(EditMode::Tile);

		// 4) World Resolution ����
		Vector3 TileSize = m_SelectComponent->GetWorldScale();
		CSceneManager::GetInst()->GetScene()->SetWorldResolution(TileSize.x, TileSize.y);
	}
	// BackGround Component ���
	else if (BackGroundWindow && m_SelectComponent->CheckType<CBackGroundComponent>())
	{
		BackGroundWindow->SetBackGround((CBackGroundComponent*)m_SelectComponent);

		// Edit Mode�� Tile Edit ���� �ٲ��ش�
		CEditorManager::GetInst()->SetEditMode(EditMode::Back);
	}
	/*
	else
	{
		TileMapWindow->SetTileMap(nullptr);
	}
	*/
}
