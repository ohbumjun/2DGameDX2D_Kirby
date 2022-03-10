#include "TileMapWindow.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "Component/TileEmptyComponent.h"
#include "Component/BackGroundComponent.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../EditorManager.h"
#include "Input.h"
#include "PathManager.h"

CTileMapWindow::CTileMapWindow()
{}

CTileMapWindow::~CTileMapWindow()
{}

void CTileMapWindow::SetTileMap(CTileEmptyComponent* TileMap)
{
	if (m_TileMapEmpty)
		return;

	m_TileMapEmpty = TileMap;

	// 전체 World Scale 정보도 세팅해야 하는가 ?

	char CurrentCountX[MAX_PATH] = {};
	sprintf_s(CurrentCountX, "%.1d", m_TileMapEmpty->GetTileCountX());
	m_CurrentTileCountX->SetText(CurrentCountX);

	char CurrentCountY[MAX_PATH] = {};
	sprintf_s(CurrentCountY, "%.1d", m_TileMapEmpty->GetTileCountY());
	m_CurrentTileCountY->SetText(CurrentCountY);

	char CurrentSizeX[MAX_PATH] = {};
	sprintf_s(CurrentCountY, "%.1f", m_TileMapEmpty->GetTileEmptySize().x);
	m_CurrentTileSizeX->SetText(CurrentCountY);

	char CurrentSizeY[MAX_PATH] = {};
	sprintf_s(CurrentSizeY, "%.1f", m_TileMapEmpty->GetTileEmptySize().y);
	m_CurrentTileSizeY->SetText(CurrentSizeY);

	// 1) Tile Map Image 정보 세팅
	if (!m_TileMapEmpty->GetTileImageMaterial()->EmptyTexture())
	{
		char ImageWorldWidth[MAX_PATH] = {};
		sprintf_s(ImageWorldWidth, "%.1f", m_TileMapEmpty->GetWorldScale().x);
		m_TileBaseScaleX->SetText(ImageWorldWidth);

		m_TileBaseScaleXInput->SetFloat(m_TileMapEmpty->GetWorldScale().x);

		char ImageWorldHeight[MAX_PATH] = {};
		sprintf_s(ImageWorldHeight, "%.1f", m_TileMapEmpty->GetWorldScale().y);
		m_TileBaseScaleY->SetText(ImageWorldHeight);

		m_TileBaseScaleYInput->SetFloat(m_TileMapEmpty->GetWorldScale().y);

		// Texture Sprite 에 세팅하기
		m_TileBaseImageSprite->SetTexture(m_TileMapEmpty->GetTileImageMaterial()->GetTexture());
	}

	// 2) Tile 정보 세팅
	// -- Tile Count
	char TileCountX[MAX_PATH] = {};
	sprintf_s(TileCountX, "%.1d", m_TileMapEmpty->GetTileCountX());
	m_CurrentTileCountX->SetText(TileCountX);
	m_TileCountX->SetInt(m_TileMapEmpty->GetTileCountX());

	char TileCountY[MAX_PATH] = {};
	sprintf_s(TileCountY, "%.1d", m_TileMapEmpty->GetTileCountY());
	m_CurrentTileCountY->SetText(TileCountY);
	m_TileCountY->SetInt(m_TileMapEmpty->GetTileCountY());

	// -- Tile  Size
	char TileSizeX[MAX_PATH] = {};
	sprintf_s(TileSizeX, "%.1f", m_TileMapEmpty->GetTileEmptySize().x);
	m_CurrentTileSizeX->SetText(TileSizeX);
	m_TileSizeX->SetFloat(m_TileMapEmpty->GetTileEmptySize().x);

	char TileSizeY[MAX_PATH] = {};
	sprintf_s(TileSizeY, "%.1f", m_TileMapEmpty->GetTileEmptySize().y);
	m_CurrentTileSizeY->SetText(TileSizeY);
	m_TileSizeY->SetFloat(m_TileMapEmpty->GetTileEmptySize().y);
}

bool CTileMapWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	CIMGUIWindow::Init();

	// ==============================
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("WorldInfo", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("World Scale", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_WorldSizeX = AddWidget<CIMGUIText>("WorldX");
	m_WorldSizeX->SetColor(255, 255, 255);
	m_WorldSizeX->SetText("0.f");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(180.f);

	m_WorldSizeY = AddWidget<CIMGUIText>("WorldY");
	m_WorldSizeY->SetColor(255, 255, 255);
	m_WorldSizeY->SetText("0.f");
	
	// ==============================

	Label = AddWidget<CIMGUILabel>("Tile Info", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Count", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_CurrentTileCountX = AddWidget<CIMGUIText>("CurCountX");
	m_CurrentTileCountX->SetSize(80.f, 40.f);
	m_CurrentTileCountX->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_CurrentTileCountY = AddWidget<CIMGUIText>("CurCountY");
	m_CurrentTileCountY->SetSize(80.f, 40.f);
	m_CurrentTileCountY->SetHideName(true);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Size", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_CurrentTileSizeX = AddWidget<CIMGUIText>("CurSizeX");
	m_CurrentTileSizeX->SetSize(80.f, 40.f);
	m_CurrentTileSizeX->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_CurrentTileSizeY = AddWidget<CIMGUIText>("CurSizeY");
	m_CurrentTileSizeY->SetSize(80.f, 40.f);
	m_CurrentTileSizeY->SetHideName(true);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

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

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Button = AddWidget<CIMGUIButton>("ResizeTileMapToBack", 200.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileMapSizeToTileBaseImage);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Tile Base Info", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("TileType", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileType = AddWidget<CIMGUIComboBox>("TileType", 120.f, 50.f);
	m_TileType->SetHideName(true);
	m_TileType->AddItem("Normal");
	m_TileType->AddItem("Wall");
	m_TileType->AddItem("Ceiling");
	m_TileType->AddItem("Water");
	m_TileType->AddItem("Block");
	m_TileType->SetSelectCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileTypeCallback);

	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);
	Label = AddWidget<CIMGUILabel>("", 0.f, 0.f);
	Label->SetColor(0, 0, 0);
	
	// ==============================

	Label = AddWidget<CIMGUILabel>("Tile Image Info", 400.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Tile Img", 195.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	Label = AddWidget<CIMGUILabel>("Tile Img Load", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	// ==============================

	m_TileBaseImageSprite = AddWidget<CIMGUIImage>("Edit Tile Image", 195.f, 195.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_TileImageLoadButton = AddWidget<CIMGUIButton>("Load Tile Img", 100.f, 30.f);
	m_TileImageLoadButton->SetClickCallback(this, &CTileMapWindow::TileBaseImageLoadButton);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Current Scale", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileBaseScaleX = AddWidget<CIMGUIText>("X");
	m_TileBaseScaleX->SetSize(80.f, 40.f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_TileBaseScaleY = AddWidget<CIMGUIText>("X");
	m_TileBaseScaleY->SetSize(80.f, 40.f);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Set Scale", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileBaseScaleXInput = AddWidget<CIMGUITextInput>("X");
	m_TileBaseScaleXInput->SetSize(70.f, 40.f);
	m_TileBaseScaleXInput->SetInt(100);
	m_TileBaseScaleXInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_TileBaseScaleYInput = AddWidget<CIMGUITextInput>("Y");
	m_TileBaseScaleYInput->SetSize(70.f, 40.f);
	m_TileBaseScaleYInput->SetInt(100);
	m_TileBaseScaleYInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(325.f);

	Button = AddWidget<CIMGUIButton>("Set Img Scale", 85.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileBaseImageScale);

	// ==============================

	Label = AddWidget<CIMGUILabel>("Set WPos", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(120.f);

	m_TileBaseWPosXInput = AddWidget<CIMGUITextInput>("WX");
	m_TileBaseWPosXInput->SetSize(70.f, 40.f);
	m_TileBaseWPosXInput->SetInt(100);
	m_TileBaseWPosXInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	m_TileBaseWPosYInput = AddWidget<CIMGUITextInput>("WY");
	m_TileBaseWPosYInput->SetSize(70.f, 40.f);
	m_TileBaseWPosYInput->SetInt(100);
	m_TileBaseWPosYInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(325.f);

	Button = AddWidget<CIMGUIButton>("Set World Pos", 85.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileBaseWorldPos);

	// =============================================
	Label = AddWidget<CIMGUILabel>("", 0, 0.f);
	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(325.f);

	Button = AddWidget<CIMGUIButton>("Set RS Scale", 85.f, 30.f);
	Button->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SetTileBaseImageResolutionScale);

	// =============================================

	Vector2 WorldResolution = CSceneManager::GetInst()->GetScene()->GetWorldResolution();

	char SizeX[MAX_PATH] = {};
	sprintf_s(SizeX, "%.f", WorldResolution.x);
	m_WorldSizeX->SetText(SizeX);

	char SizeY[MAX_PATH] = {};
	sprintf_s(SizeY, "%.f", WorldResolution.y);
	m_WorldSizeY->SetText(SizeY);

	// Default Value 들 세팅
	m_TileCountX->SetInt(100);
	m_TileCountY->SetInt(100);
	m_TileSizeX->SetFloat(160.f);
	m_TileSizeY->SetFloat(80.f);

	// Tile Count Info Display
	char CurrentCountX[MAX_PATH] = {};
	sprintf_s(CurrentCountX, "%.1d", 100);
	m_CurrentTileCountX->SetText(CurrentCountX);

	char CurrentCountY[MAX_PATH] = {};
	sprintf_s(CurrentCountY, "%.1d", 100);
	m_CurrentTileCountY->SetText(CurrentCountY);

	char CurrentSizeX[MAX_PATH] = {};
	sprintf_s(CurrentSizeX, "%.1f", 160.f);
	m_CurrentTileSizeX->SetText(CurrentSizeX);

	char CurrentSizeY[MAX_PATH] = {};
	sprintf_s(CurrentSizeY, "%.1f", 80.f);
	m_CurrentTileSizeY->SetText(CurrentSizeY);

	// Tile Base Info Display
	char TileBaseSizeX[MAX_PATH] = {};
	sprintf_s(TileBaseSizeX, "%.1f", 0.f);
	m_TileBaseScaleX->SetText(TileBaseSizeX);

	char TileBaseSizeY[MAX_PATH] = {};
	sprintf_s(TileBaseSizeY, "%.1f", 80.f);
	m_TileBaseScaleY->SetText(TileBaseSizeY);

	// Tile Base Input 세팅
	m_TileBaseScaleXInput->SetFloat(0.f);
	m_TileBaseScaleYInput->SetFloat(0.f);


	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	EditMode CurrentEditMode = CEditorManager::GetInst()->GetEditMode();

	if (CEditorManager::GetInst()->GetEditMode() == EditMode::Tile && m_TileMapEmpty)
	{
		m_TileMapEmpty->EnableEditMode(true);

		// Mouse를 눌렀다면 
		if (CEditorManager::GetInst()->GetMousePush())
		{
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

			CTileEmpty* Tile = m_TileMapEmpty->GetTileEmpty(Vector3(MouseWorldPos.x, MouseWorldPos.y, 0.f));

			if (!Tile) // 해당 위치에 타일이 존재하지 않는다면 처리 X
				return;

			int TypeIndex = m_TileType->GetSelectIndex();

			if (TypeIndex < 0 || TypeIndex >= (int)Tile_Type::End)
				return;

			Tile->SetTileType((Tile_Type)TypeIndex);
		}
	}
}

void CTileMapWindow::CreateTile()
{
	if (!m_TileMapEmpty)
		return;

	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

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

	// 현재 정보 Update

	char CurrentCountX[MAX_PATH] = {};
	sprintf_s(CurrentCountX, "%.1d", CountX);
	m_CurrentTileCountX->SetText(CurrentCountX);

	char CurrentCountY[MAX_PATH] = {};
	sprintf_s(CurrentCountY, "%.1d", CountY);
	m_CurrentTileCountY->SetText(CurrentCountY);

	char CurrentSizeX[MAX_PATH] = {};
	sprintf_s(CurrentCountY, "%.1f", SizeX);
	m_CurrentTileSizeX->SetText(CurrentCountY);

	char CurrentSizeY[MAX_PATH] = {};
	sprintf_s(CurrentSizeY, "%.1f", SizeY);
	m_CurrentTileSizeY->SetText(CurrentSizeY);


	// Tile 생성
	// m_TileMapEmpty->CreateTile(Shape,CountX, CountY, Vector3(SizeX, SizeY, 1.f));
	m_TileMapEmpty->CreateTileEmpty(CountX, CountY, Vector3(SizeX, SizeY, 1.f));

}

void CTileMapWindow::SetEditModeCallback(int Index, const char* Name)
{
	// Editor 에 Tile Edit 이라고 세팅하기
	CEditorManager::GetInst()->SetEditMode(EditMode::Tile);
}

void CTileMapWindow::SetTileTypeCallback(int Index, const char* Name)
{}

void CTileMapWindow::SetTileMapSizeToTileBaseImage()
{
	if (!m_TileMapEmpty)
		return;

	if (!m_TileMapEmpty->GetTileImageMaterial() || m_TileMapEmpty->GetTileCount() == 0)
		return;

	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

	Vector2 BackGroundSize = Vector2(m_TileMapEmpty->GetWorldScale().x, m_TileMapEmpty->GetWorldScale().y);

	float TileSizeX = BackGroundSize.x /(float) m_TileMapEmpty->GetTileCountX();
	float TileSizeY = BackGroundSize.y /(float) m_TileMapEmpty->GetTileCountY();

	// 현재 정보 Update

	char CurrentSizeX[MAX_PATH] = {};
	sprintf_s(CurrentSizeX, "%.1f", TileSizeX);
	m_CurrentTileSizeX->SetText(CurrentSizeX);

	char CurrentSizeY[MAX_PATH] = {};
	sprintf_s(CurrentSizeY, "%.1f", TileSizeY);
	m_CurrentTileSizeY->SetText(CurrentSizeY);

	m_TileMapEmpty->SetTileDefaultSize(TileSizeX, TileSizeY);
}


void CTileMapWindow::TileBaseImageLoadButton()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

	if (!m_TileMapEmpty)
		return;

	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OpenFile);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0DDSFILE\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*jpg\0JPEGFile\0*.jpeg\0BMPFile\0*bmp");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		// File 이름 뽑아내기
		TCHAR FileName[MAX_PATH] = {};

		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, nullptr, 0);

		char ConvertFileName[MAX_PATH] = {};

		int  Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, nullptr, nullptr);

		// FullPath 먼저 만들기

		// BackMaterial 이 만들어져 있지 않다면, 만든다.
		if (!m_TileMapEmpty->GetTileImageMaterial())
		{
			CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("TileBaseMaterial");

			CMaterial* BackMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("TileBaseMaterial");

			BackMaterial->SetShader("Mesh2DShader");

			m_TileMapEmpty->SetTileImageMaterial(BackMaterial);
		}

		if (m_TileMapEmpty->GetTileImageMaterial()->EmptyTexture())
		{
			m_TileMapEmpty->GetTileImageMaterial()->AddTextureFullPath(0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		}
		else
		{
			m_TileMapEmpty->GetTileImageMaterial()->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		}

		m_TileMapEmpty->SetWorldScale((float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureWidth(),
			(float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureHeight(), 1.f);

		char TileImgWidth[MAX_PATH] = {};
		sprintf_s(TileImgWidth, "%1.f", (float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureWidth());
		m_TileBaseScaleX->SetText(TileImgWidth);

		char TileImgHeight[MAX_PATH] = {};
		sprintf_s(TileImgHeight, "%1.f", (float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureHeight());
		m_TileBaseScaleX->SetText(TileImgHeight);

		m_TileBaseScaleXInput->SetFloat((float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureWidth());
		m_TileBaseScaleYInput->SetFloat((float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureHeight());

		m_TileBaseImageSprite->SetTexture(m_TileMapEmpty->GetTileImageMaterial()->GetTexture());
		m_TileBaseImageSprite->SetImageStart(Vector2(0.f, 0.f));
		m_TileBaseImageSprite->SetImageEnd(Vector2((float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureWidth(),
			(float)m_TileMapEmpty->GetTileImageMaterial()->GetTextureHeight()));
	}
}

void CTileMapWindow::SetTileBaseImageScale()
{
	if (!m_TileMapEmpty)
		return;

	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

	if (!m_TileBaseScaleXInput->FloatEmpty() && !m_TileBaseScaleYInput->FloatEmpty())
	{
		m_TileMapEmpty->SetWorldScale(m_TileBaseScaleXInput->GetValueFloat(), m_TileBaseScaleYInput->GetValueFloat(), 1.f);

		// World Scale 도 세팅하기
		CSceneManager::GetInst()->GetScene()->SetWorldResolution(m_TileBaseScaleXInput->GetValueFloat(), m_TileBaseScaleYInput->GetValueFloat());

		char ImageWorldWidth[MAX_PATH] = {};
		sprintf_s(ImageWorldWidth, "%.1f", m_TileBaseScaleXInput->GetValueFloat());
		m_TileBaseScaleX->SetText(ImageWorldWidth);
		m_WorldSizeX->SetText(ImageWorldWidth);

		char ImageWorldHeight[MAX_PATH] = {};
		sprintf_s(ImageWorldHeight, "%.1f", m_TileBaseScaleYInput->GetValueFloat());
		m_TileBaseScaleY->SetText(ImageWorldHeight);
		m_WorldSizeY->SetText(ImageWorldHeight);
	}
}

void CTileMapWindow::SetTileBaseWorldPos()
{
	if (!m_TileMapEmpty)
		return;

	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

	if (!m_TileBaseWPosXInput->FloatEmpty() && !m_TileBaseWPosYInput->FloatEmpty())
	{
		m_TileMapEmpty->SetWorldPos(m_TileBaseWPosXInput->GetValueFloat(),
					m_TileBaseWPosYInput->GetValueFloat(), 1.f);
	}
}

void CTileMapWindow::SetTileBaseImageResolutionScale()
{
	Resolution RS = CEngine::GetInst()->GetResolution();

	if (!m_TileMapEmpty)
		return;

	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Tile)
		return;

	if (!m_TileBaseScaleXInput->FloatEmpty() && !m_TileBaseScaleYInput->FloatEmpty())
	{
		m_TileMapEmpty->SetWorldScale((float)RS.Width, (float)RS.Height, 1.f);
	}
}
