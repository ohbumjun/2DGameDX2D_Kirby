#include "TileMapWindow.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "Component/TileMapComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../EditorManager.h"
#include "Input.h"

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
	m_TileShapeCombo->AddItem("사각형");
	m_TileShapeCombo->AddItem("마름모");

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
	m_TileCountX->SetTextType(ImGuiText_Type::Int);

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
	m_TileCountY->SetTextType(ImGuiText_Type::Int);

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
	m_TileSizeX->SetTextType(ImGuiText_Type::Float);

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
	m_TileSizeY->SetTextType(ImGuiText_Type::Float);

	// ==============================

	CIMGUIButton* Button = AddWidget<CIMGUIButton>("CreateTileMap", 200.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::CreateTile);

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

	m_TileType = AddWidget<CIMGUIComboBox>("TileType", 100.f, 50.f);
	m_TileType->SetHideName(true);
	m_TileType->AddItem("Normal");
	m_TileType->AddItem("Wall");

	// ==============================

	Label = AddWidget<CIMGUILabel>("TileEdit", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileEdit = AddWidget<CIMGUIComboBox>("TileEditMode", 100.f, 50.f);
	m_TileEdit->SetHideName(true);
	m_TileEdit->AddItem("Type");
	m_TileEdit->AddItem("Frame");
	m_TileEdit->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::SetEditModeCallback);

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
	m_TileFrameStartX->SetTextType(ImGuiText_Type::Float);

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
	m_TileFrameStartY->SetTextType(ImGuiText_Type::Float);

	// ==============================

	Label = AddWidget<CIMGUILabel>("FrameEndX", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileFrameEndX = AddWidget<CIMGUITextInput>("FrameEndX");
	m_TileFrameEndX->SetSize(80.f, 40.f);
	m_TileFrameEndX->SetInt(100);
	m_TileFrameEndX->SetHideName(true);
	m_TileFrameEndX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("FrameEndY", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	m_TileFrameEndY = AddWidget<CIMGUITextInput>("FrameEndY");
	m_TileFrameEndY->SetSize(80.f, 40.f);
	m_TileFrameEndY->SetInt(100);
	m_TileFrameEndY->SetHideName(true);
	m_TileFrameEndY->SetTextType(ImGuiText_Type::Float);

	// ==============================

	Button = AddWidget<CIMGUIButton>("SetDefaultFrame", 200.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetDefaultFrame);

	// Default Value 들 세팅
	m_TileCountX->SetInt(100);
	m_TileCountY->SetInt(100);
	m_TileSizeX->SetFloat(160.f);
	m_TileSizeY->SetFloat(80.f);

	m_TileFrameStartX->SetFloat(160.f);
	m_TileFrameStartY->SetFloat(80.f);
	m_TileFrameEndX->SetFloat(320.f);
	m_TileFrameEndY->SetFloat(160.f);

	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	if (CEditorManager::GetInst()->GetEditMode() == EditMode::Tile && m_TileMap)
	{
		m_TileMap->EnableEditMode(true);

		// Mouse를 눌렀다면 
		if (CEditorManager::GetInst()->GetMousePush())
		{
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

			CTile* Tile = m_TileMap->GetTile(Vector3(MouseWorldPos.x, MouseWorldPos.y, 0.f));

		}
	}
}

void CTileMapWindow::CreateTile()
{
	if (!m_TileMap)
		return;

	// Tile Shape
	int TileShapeIndex = m_TileShapeCombo->GetSelectIndex();

	if (TileShapeIndex < 0 || TileShapeIndex >= (int)Tile_Shape::End)
		return;

	Tile_Shape Shape = (Tile_Shape)TileShapeIndex;

	// CountX, CountY
	int CountX = m_TileCountX->GetValueInt();
	int CountY = m_TileCountY->GetValueInt();

	if (CountX <= 0 || CountY <= 0)
		return;

	// Size
	float SizeX = m_TileSizeX->GetValueFloat();
	float SizeY = m_TileSizeY->GetValueFloat();

	if (SizeX <= 0 || SizeY <= 0)
		return;

	// Tile 생성
	m_TileMap->CreateTile(Shape, CountX, CountY, Vector3(SizeX, SizeY, 1.f));

	// Material 및 Texture 세팅하기
	CTexture* Texture = nullptr;

	switch(Shape)
	{
	case Tile_Shape::Rect :
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRectTile",
				TEXT("Floors.png"));

			Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture("DefaultRectTile");
		}
		break;
	case Tile_Shape::Rhombus :
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRhombusTile",
				TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

			Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture("DefaultRhombusTile");
		}
		break;
	}

	CMaterial* Material = m_TileMap->GetTileMaterial();

	if (Material->EmptyTexture())
		Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);
	else
		Material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);

	// TileMapComponent 의 상수버퍼에 전체 ImageSize를 지정해주도록 세팅한다.
	m_TileMap->SetImageSizeToCBuffer();

}

void CTileMapWindow::SetDefaultFrame()
{
	if (!m_TileMap)
		return;

	float StartFrameX = m_TileFrameStartX->GetValueFloat();
	float StartFrameY = m_TileFrameStartY->GetValueFloat();

	float EndFrameX = m_TileFrameEndX->GetValueFloat();
	float EndFrameY = m_TileFrameEndY->GetValueFloat();

	if (StartFrameX < 0.f || StartFrameY < 0.f || EndFrameX < 0.f || EndFrameY < 0.f)
		return;

	m_TileMap->SetTileDefaultFrame(StartFrameX, StartFrameY, EndFrameX, EndFrameY);
}

void CTileMapWindow::SetEditModeCallback(int Index, const char* Name)
{
	m_EditMode = (Tile_EditMode)Index;
}
