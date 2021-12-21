#include "DetailInfoWindow.h"
#include "IMGUILabel.h"
#include "IMGUISameLine.h"
#include "IMGUITextInput.h"
#include "ObjectHierarchy.h"
#include "GameObject/GameObject.h"
#include "Scene/SceneResource.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../EditorManager.h"
#include "IMGUIListBox.h"

CDetailInfoWindow::CDetailInfoWindow()
{
}

CDetailInfoWindow::~CDetailInfoWindow()
{
}

bool CDetailInfoWindow::Init()
{
	CIMGUIWindow::Init();

	// ===============================================
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Pos", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ImGui::SetCursorScreenPos(ImVec2(100.f, 100.f));

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosX = AddWidget<CIMGUITextInput>("PosX", 80.f, 30.f);
	m_PosX->SetHideName(true);
	m_PosX->SetCallback(this, &CDetailInfoWindow::SetPositionXCallback);
	m_PosX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosY = AddWidget<CIMGUITextInput>("PosY", 80.f, 30.f);
	m_PosY->SetHideName(true);
	m_PosY->SetCallback(this, &CDetailInfoWindow::SetPositionYCallback);
	m_PosY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosZ = AddWidget<CIMGUITextInput>("PosZ", 80.f, 30.f);
	m_PosZ->SetHideName(true);
	m_PosZ->SetCallback(this, &CDetailInfoWindow::SetPositionZCallback);
	m_PosZ->SetTextType(ImGuiText_Type::Float);

	// ===============================================
	Label = AddWidget<CIMGUILabel>("Rot", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotX = AddWidget<CIMGUITextInput>("RotX", 80.f, 30.f);
	m_RotX->SetHideName(true);
	m_RotX->SetCallback(this, &CDetailInfoWindow::SetRotationXCallback);
	m_RotX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotY = AddWidget<CIMGUITextInput>("RotY", 80.f, 30.f);
	m_RotY->SetHideName(true);
	m_RotY->SetCallback(this, &CDetailInfoWindow::SetRotationYCallback);
	m_RotY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotZ = AddWidget<CIMGUITextInput>("RotZ", 80.f, 30.f);
	m_RotZ->SetHideName(true);
	m_RotZ->SetCallback(this, &CDetailInfoWindow::SetRotationZCallback);
	m_RotZ->SetTextType(ImGuiText_Type::Float);

	// ==============================================

	Label = AddWidget<CIMGUILabel>("Scale", 40.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleX = AddWidget<CIMGUITextInput>("ScaleX", 80.f, 30.f);
	m_ScaleX->SetHideName(true);
	m_ScaleX->SetCallback(this, &CDetailInfoWindow::SetScalingXCallback);
	m_ScaleX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleY = AddWidget<CIMGUITextInput>("ScaleY", 80.f, 30.f);
	m_ScaleY->SetHideName(true);
	m_ScaleY->SetCallback(this, &CDetailInfoWindow::SetScalingYCallback);
	m_ScaleY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 20.f, 20.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScaleZ = AddWidget<CIMGUITextInput>("ScaleZ", 80.f, 30.f);
	m_ScaleZ->SetHideName(true);
	m_ScaleZ->SetCallback(this, &CDetailInfoWindow::SetScalingZCallback);
	m_ScaleZ->SetTextType(ImGuiText_Type::Float);

	return true;
}

void CDetailInfoWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CDetailInfoWindow::SetPositionXCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || !Hierarchy->GetObjectListBox()->GetSelectIndex() < -1)
		return;

	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;

	Vector3 ObjectPos = Object->GetWorldPos();
	float NewXPos = m_PosX->GetValueFloat();
	Object->SetWorldPos(NewXPos, ObjectPos.y, ObjectPos.z);
}

void CDetailInfoWindow::SetPositionYCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	// Object 선택 X
	if (!Hierarchy || !Hierarchy->GetObjectListBox()->GetSelectIndex() < -1)
		return;

	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	// 해당 이름으로 된 Object가 없다면 
	if (!Object)
		return;

	Vector3 ObjectPos = Object->GetWorldPos();
	float NewYPos = m_PosY->GetValueFloat();
	Object->SetWorldPos(ObjectPos.x, NewYPos, ObjectPos.z);
}

void CDetailInfoWindow::SetPositionZCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || !Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;


	Vector3 ObjectPos = Object->GetWorldPos();
	float NewZPos = m_PosZ->GetValueFloat();
	Object->SetWorldPos(ObjectPos.x, ObjectPos.y, NewZPos);
}

void CDetailInfoWindow::SetRotationXCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || !Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;

	Object->SetWorldRotationX(m_RotX->GetValueFloat());
}

void CDetailInfoWindow::SetRotationYCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || !Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	
	Object->SetWorldRotationY(m_RotY->GetValueFloat());
}

void CDetailInfoWindow::SetRotationZCallback()
{
	// 선택한 GameObject + Component가 존재해야 한다.
	CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

	if (!Hierarchy || !Hierarchy->GetObjectListBox()->GetSelectIndex() < 0)
		return;

	const char* ObjectName = Hierarchy->GetObjectListBox()->GetSelectItem().c_str();
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindGameObject(ObjectName);

	if (!Object)
		return;
	
	Object->SetWorldRotationZ(m_RotZ->GetValueFloat());
}

void CDetailInfoWindow::SetScalingXCallback()
{
}

void CDetailInfoWindow::SetScalingYCallback()
{
}

void CDetailInfoWindow::SetScalingZCallback()
{
}