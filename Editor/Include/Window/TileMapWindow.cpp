#include "TileMapWindow.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"

CTileMapWindow::CTileMapWindow()
{}

CTileMapWindow::~CTileMapWindow()
{}

bool CTileMapWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	CIMGUIWindow::Init();
	// ==============================
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("TileMapInfo", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);
	// ==============================
	Label = AddWidget<CIMGUILabel>("TileMapShape", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileShapeCombo = AddWidget<CIMGUIComboBox>("TileShape", 100.f, 50.f);
	m_TileShapeCombo->SetHideName(true);
	m_TileShapeCombo->AddItem("마름모");
	m_TileShapeCombo->AddItem("사각형");
	// ==============================

	Label = AddWidget<CIMGUILabel>("CountX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileCountX = AddWidget<CIMGUITextInput>("TileCountX");
	m_TileCountX->SetSize(80.f, 40.f);
	m_TileCountX->SetInt(100);
	m_TileCountX->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("CountY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileCountY = AddWidget<CIMGUITextInput>("TileCountY");
	m_TileCountY->SetSize(80.f, 40.f);
	m_TileCountY->SetInt(100);
	m_TileCountY->SetHideName(true);

	// ==============================
	Label = AddWidget<CIMGUILabel>("SizeX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileSizeX = AddWidget<CIMGUITextInput>("SizeX");
	m_TileSizeX->SetSize(80.f, 40.f);
	m_TileSizeX->SetInt(100);
	m_TileSizeX->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("SizeY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileSizeY = AddWidget<CIMGUITextInput>("SizeY");
	m_TileSizeY->SetSize(80.f, 40.f);
	m_TileSizeY->SetInt(100);
	m_TileSizeY->SetHideName(true);

	// ==============================
	CIMGUIButton* Button = AddWidget<CIMGUIButton>("CreateTileMap", 200.f, 30.f);

	// ==============================
	Label = AddWidget<CIMGUILabel>("TileEditInfo", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);
	// ==============================
	Label = AddWidget<CIMGUILabel>("TileType", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileType = AddWidget<CIMGUIComboBox>("TileShape", 100.f, 50.f);
	m_TileType->SetHideName(true);
	m_TileType->AddItem("Normal");
	m_TileType->AddItem("Wall");
	// ==============================
	Label = AddWidget<CIMGUILabel>("TileEdit", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileEdit = AddWidget<CIMGUIComboBox>("TileShape", 100.f, 50.f);
	m_TileEdit->SetHideName(true);
	m_TileEdit->AddItem("True");
	m_TileEdit->AddItem("False");

	// ==============================

	Label = AddWidget<CIMGUILabel>("FrameStartX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileFrameStartX = AddWidget<CIMGUITextInput>("FrameStartX");
	m_TileFrameStartX->SetSize(80.f, 40.f);
	m_TileFrameStartX->SetInt(100);
	m_TileFrameStartX->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("FrameStartY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileFrameStartY = AddWidget<CIMGUITextInput>("FrameStartY");
	m_TileFrameStartY->SetSize(80.f, 40.f);
	m_TileFrameStartY->SetInt(100);
	m_TileFrameStartY->SetHideName(true);

	// ==============================
	Label = AddWidget<CIMGUILabel>("FrameEndX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileFrameEndX = AddWidget<CIMGUITextInput>("SizeX");
	m_TileFrameEndX->SetSize(80.f, 40.f);
	m_TileFrameEndX->SetInt(100);
	m_TileFrameEndX->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("FrameEndY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileFrameEndY = AddWidget<CIMGUITextInput>("SizeY");
	m_TileFrameEndY->SetSize(80.f, 40.f);
	m_TileFrameEndY->SetInt(100);
	m_TileFrameEndY->SetHideName(true);

	// ==============================
	Button = AddWidget<CIMGUIButton>("SetDefaultFrame", 200.f, 30.f);


	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}
