#include "LineEditWindow.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "Component/BackGroundComponent.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

CLineEditWindow::CLineEditWindow()
{}

CLineEditWindow::~CLineEditWindow()
{}

void CLineEditWindow::SetBackGround(CBackGroundComponent* BackGround)
{
	if (m_BackGround)
		return;

	// 2) BackMaterial ¼¼ÆÃ 
	std::string BackMaterialName = BackGround->GetBackGroundMaterial()->GetName();

	m_BackGround = BackGround;


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

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("Line Create", 95.f, 30.f);
	Button->SetClickCallback<CLineEditWindow>(this, &CLineEditWindow::SetLineCreateMode);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(105.f);

	Button = AddWidget<CIMGUIButton>("Line Edit", 95.f, 30.f);
	Button->SetClickCallback<CLineEditWindow>(this, &CLineEditWindow::SetLineEditMode);

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

	Label = AddWidget<CIMGUILabel>("Set Ratio", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_LineSlope = AddWidget<CIMGUIText>("Slope");
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
	// m_CreatedLineListBox->SetSelectCallback(this, &CObjectHierarchy::SeeObjectList);

	return true;
}

void CLineEditWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CLineEditWindow::SetEditModeCallback(int Index, const char* Name)
{
	CEditorManager::GetInst()->SetEditMode(EditMode::Back);
}

void CLineEditWindow::SetLineCreateMode()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::LineCreate);
}

void CLineEditWindow::SetLineEditMode()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::LineEdit);
}
