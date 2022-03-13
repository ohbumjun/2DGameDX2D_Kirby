#include "LineEditWindow.h"

#include <Scene/CameraManager.h>

#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "ObjectHierarchy.h"
#include "Component/BackGroundComponent.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../Object/Line.h"
#include "../Object/LineContainer.h"
#include "../Editor.h"

CLineEditWindow::CLineEditWindow()
{}

CLineEditWindow::~CLineEditWindow()
{}

void CLineEditWindow::SetLineContainer(CGameObject* LineContainer)
{
	m_LineContainer = (CLineContainer*)LineContainer;

	/*
	if (m_LineContainer->GetChildCount() == 0)
	{
		// - 기본적으로 Line Object 를 하나를 생성하고
		CLine* CreatedLine =  CSceneManager::GetInst()->GetScene()->CreateGameObject<CLine>("Line0");

		// - Line Object 를 Line Container의 자식으로 세팅하고
		m_LineContainer->AddChildGameObject(CreatedLine);

		// - 첫 Line Object 정보를 Line Window에다가 세팅한다.
		// ex) st , ed pos, slope
		SetLineInfo(CreatedLine);

		// Line Object List에 추가한다.
		m_CreatedLineListBox->AddItem(CreatedLine->GetName());

		m_CreatedLineListBox->SetSelectIndex(0);

		m_SelectLine = CreatedLine;
	}
	*/
}

void CLineEditWindow::SetLineInfo(CLine* Line)
{
	if (!Line)
		return;

	// ex) st , ed pos, slope
	Vector3 StartPos = Line->GetFinalWorldLeftPos();
	Vector3 EndPos = Line->GetFinalWorldRightPos();

	// 아직 세팅된 적이 없다면 
	if (StartPos.x == -FLT_MAX)
		return;

	char StartPosX[MAX_PATH] = {};
	sprintf_s(StartPosX, "%.1f", StartPos.x);
	m_LineStartPosX->SetText(StartPosX);

	char StartPosY[MAX_PATH] = {};
	sprintf_s(StartPosY, "%.1f", StartPos.y);
	m_LineStartPosY->SetText(StartPosY);

	char EndPosX[MAX_PATH] = {};
	sprintf_s(EndPosX, "%.1f", EndPos.x);
	m_LineEndPosX->SetText(EndPosX);

	char EndPosY[MAX_PATH] = {};
	sprintf_s(EndPosY, "%.1f", EndPos.y);
	m_LineEndPosY->SetText(EndPosY);
}

void CLineEditWindow::SetLineDescription(const Vector3& FinalLeftPos, const Vector3& FinalRightPos, float Slope)
{
	char StartPosX[MAX_PATH] = {};
	sprintf_s(StartPosX, "%.1f", FinalLeftPos.x);
	m_LineStartPosX->SetText(StartPosX);

	char StartPosY[MAX_PATH] = {};
	sprintf_s(StartPosY, "%.1f", FinalLeftPos.y);
	m_LineStartPosY->SetText(StartPosY);

	char EndPosX[MAX_PATH] = {};
	sprintf_s(EndPosX, "%.1f", FinalRightPos.x);
	m_LineEndPosX->SetText(EndPosX);

	char EndPosY[MAX_PATH] = {};
	sprintf_s(EndPosY, "%.1f", FinalRightPos.y);
	m_LineEndPosY->SetText(EndPosY);

	char SlopeText[MAX_PATH] = {};
	sprintf_s(SlopeText, "%.1f", Slope);
	m_LineSlope->SetText(SlopeText);
}

bool CLineEditWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Line Edit Info", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Select Mode", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("Create", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("Create Mode", 95.f, 30.f);
	Button->SetClickCallback<CLineEditWindow>(this, &CLineEditWindow::SetLineCreateMode);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(109.f);

	Button = AddWidget<CIMGUIButton>("Edit Mode", 95.f, 30.f);
	Button->SetClickCallback<CLineEditWindow>(this, &CLineEditWindow::SetLineEditMode);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Button = AddWidget<CIMGUIButton>("Create Line", 95.f, 30.f);
	Button->SetClickCallback<CLineEditWindow>(this, &CLineEditWindow::CreateNewLine);

	Label = AddWidget<CIMGUILabel>("", 0, 0);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.f, 0.f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("", 0, 0);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.f, 0.f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Line Info", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Start Pos", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_LineStartPosX = AddWidget<CIMGUIText>("St X");
	m_LineStartPosX->SetSize(80.f, 40.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_LineStartPosY = AddWidget<CIMGUIText>("Back X");
	m_LineStartPosY->SetSize(80.f, 40.f);

	char StartPosX[MAX_PATH] = {};
	sprintf_s(StartPosX, "%.1f", 0.f);
	m_LineStartPosX->SetText(StartPosX);

	char StartPosY[MAX_PATH] = {};
	sprintf_s(StartPosY, "%.1f", 00.f);
	m_LineStartPosY->SetText(StartPosY);

	// ==============================

	Label = AddWidget<CIMGUILabel>("End Pos", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_LineEndPosX = AddWidget<CIMGUIText>("Ed X");
	m_LineEndPosX->SetSize(80.f, 40.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_LineEndPosY = AddWidget<CIMGUIText>("Ed Y");
	m_LineEndPosY->SetSize(80.f, 40.f);

	char EndPosX[MAX_PATH] = {};
	sprintf_s(EndPosX, "%.1f", 0.f);
	m_LineEndPosX->SetText(EndPosX);

	char EndPosY[MAX_PATH] = {};
	sprintf_s(EndPosY, "%.1f", 00.f);
	m_LineEndPosY->SetText(EndPosY);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Slope", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_LineSlope = AddWidget<CIMGUIText>("Slope Text");
	m_LineSlope->SetSize(80.f, 40.f);

	char Slope[MAX_PATH] = {};
	sprintf_s(Slope, "%.1f", 00.f);
	m_LineSlope->SetText(Slope);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Created Line", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	m_CreatedLineListBox = AddWidget<CIMGUIListBox>("ObjectList", 195.f, 100.f);
	m_CreatedLineListBox->SetHideName(true);
	m_CreatedLineListBox->SetPageItemCount(8);
	m_CreatedLineListBox->SetSelectCallback(this, &CLineEditWindow::SelectLineObject);

	return true;
}

void CLineEditWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CLineEditWindow::SelectLineObject(int Index, const char* Name)
{
	m_SelectLine = CSceneManager::GetInst()->GetScene()->FindGameObject(Name);

	if (!m_SelectLine)
		return;

	SetLineInfo((CLine*)m_SelectLine);
}

void CLineEditWindow::SetLineCreateMode()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::LineCreate);
}

void CLineEditWindow::SetLineEditMode()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::LineEdit);
}

void CLineEditWindow::CreateNewLine()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::LineCreate)
		return;

	if (!m_LineContainer)
		return;

	std::string NewLineName = "Line" + GetRandomString();

	// - 기본적으로 Line Object 를 하나를 생성하고
	CLine* CreatedLine = CSceneManager::GetInst()->GetScene()->CreateGameObject<CLine>(NewLineName);

	Resolution RS = CEngine::GetInst()->GetResolution();

	Vector3 CameraWorldPos = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	CreatedLine->SetWorldPos(Vector3((float)RS.Width * 0.5f + CameraWorldPos.x , 
														(float)RS.Height * 0.5f + CameraWorldPos.y,
														CameraWorldPos.z));

	// - Line Object 를 Line Container의 자식으로 세팅하고
	m_LineContainer->AddChildGameObject(CreatedLine);

	// - 첫 Line Object 정보를 Line Window에다가 세팅한다.
	// ex) st , ed pos, slope
	SetLineInfo(CreatedLine);

	// Line Object List에 추가한다.
	m_CreatedLineListBox->AddItem(NewLineName);

	m_CreatedLineListBox->SetSelectIndex(m_CreatedLineListBox->GetItemCount() - 1);

	// 생성된 Object List 에서도 추가되게 한다.
	CIMGUIListBox* CreatedObjList = CEditorManager::GetInst()->GetObjectHierarchy()->GetCreatedObjectListBox();

	CreatedObjList->AddItem(NewLineName);

	CreatedObjList->SetSelectIndex(CreatedObjList->GetItemCount() - 1);
}
